// ASSIGNMENT 1 - netMonitor.cpp - Server code for multiple connections
// Authors: Andre Valle-Rosa /Yathavan Parameshwaran

// TODO LIST:
// SERVER netMonitor
// 3 - Todo client "Done" message (see documentation page 4)


// ATTENTION: RUN AS SUDO OTHERWISE THE SET LINK UP WILL NOT RUN.

#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>
#include <net/if.h>
#include <ifaddrs.h>

using namespace std;

char socket_path[]="/tmp/master";
const int BUF_LEN=200;

int MAX_CLIENTS=2;
bool is_running = true;
bool isParent = true;

//--------------------------------------------------
// SIGNAL HANDLER TO QUIT CODE
//--------------------------------------------------
static void signalHandler(int signum)
{
    switch(signum) {
        case SIGINT:
            cout<<"signalHandler("<<signum<<"): SIGINT"<<endl;
            is_running=false;
            break;
       default:
            cout<<"signalHandler("<<signum<<"): unknown"<<endl;
    }
}
//---------------------------------------------------

//---------------------------------------------------
int main(int argc, char** argv) 
{
    //Create the socket for inter-process communications
    struct sockaddr_un addr;
    char buf[BUF_LEN];
    int len;
    int master_fd,max_fd,cl[MAX_CLIENTS],rc;
    fd_set active_fd_set;
    fd_set read_fd_set;
    int ret;
    int numClients=0;

    //Set up a signal handler to terminate the program gracefully
    struct sigaction action;
    action.sa_handler = signalHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, NULL);

    //----------------------------  
    // 0. GET ARGUMENTS WITH INTERFACE NAMES 
    MAX_CLIENTS = argc -1;

    if (MAX_CLIENTS == 0) {
        cout << "insert name of interfaces as arguments\n";
        exit (0);
    }
    
    cout << "number of interfaces:" << MAX_CLIENTS << "\n";
    string clNames[MAX_CLIENTS]; // list of interfaces

    for (int i = 1; i < argc; i++) {
        clNames[i-1] = argv[i];
        cout << "interface "  << i << " " << argv[i] << endl;
    }
    //----------------------------

    //----------------------------
    // 1. CREATE SOCKET
    memset(&addr, 0, sizeof(addr));
    if ( (master_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        cout << "server: " << strerror(errno) << endl;
        exit(-1);
    }
    addr.sun_family = AF_UNIX;
    //----------------------------

    //----------------------------
    // 2. SET SOCKET TO FILE
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    unlink(socket_path);
    //----------------------------

    //----------------------------
    // 3. BIND SOCKET
    if (bind(master_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        cout << "server: " << strerror(errno) << endl;
        close(master_fd);
        exit(-1);
    }
    //----------------------------

    //----------------------------
    // 4. LISTEN FOR CLIENTS
    cout<<"Waiting for the client..."<<endl;
    if (listen(master_fd, 5) == -1) {
        cout << "server: " << strerror(errno) << endl;
        unlink(socket_path);
        close(master_fd);
        exit(-1);
    }
    //----------------------------

    //----------------------------
    // 5. START CLIENTS
    // 5.1 Loop to reate client with parameter as name
    // COMMENT HERE TO RUN MANUAL VERSION
    cout << "starting clients " << endl;
    for (int i = 1; i < argc; i++) {
        char cmd[50];
        strcpy (cmd, "sudo systemd-run ./ifMonitor ");      
        strcat (cmd, argv[i]);
        cout << "cmd: " << cmd << endl;
        int status = system(cmd);
        cout << "status:" << status << endl;
    }

    //----------------------------

    FD_ZERO(&read_fd_set);
    FD_ZERO(&active_fd_set);
    //Add the master_fd to the socket set
    FD_SET(master_fd, &active_fd_set);
    max_fd = master_fd;//We will select from max_fd+1 sockets (plus one is due to a new connection)
    is_running=true;

    //---------------------------------------------------------------------------------
    while(is_running) {
        //Block until an input arrives on one or more sockets
        read_fd_set = active_fd_set;
        ret=select(max_fd+1, &read_fd_set, NULL, NULL, NULL);//Select from up to max_fd+1 sockets
        if (ret < 0) {
            cout << "server: " << strerror(errno) << endl;
        } else {//Service all the sockets with input pending
            
            if (FD_ISSET (master_fd, &read_fd_set))
            { // 6. NEW CLIENT ARRIVES

                //----------------------------------------
                // 6.1 ACCEPT CLIENT
                cl[numClients] = accept(master_fd, NULL, NULL);
                if (cl[numClients] < 0) {
                    cout << "server: " << strerror(errno) << endl;
                } else {
                    cout<<"Server: incoming connection "<<cl[numClients]<<endl;
                    FD_SET (cl[numClients], &active_fd_set);//Add the new connection to the set
                    if(max_fd<cl[numClients]) max_fd=cl[numClients];//Update the maximum fd
                    ++numClients;
                }
                //-----------------------------------------

            }
            else // 7. AN ALREADY CONNECTED CLIENT SEND MESSAGE
            {
                for (int i = 0; i < numClients; ++i) {//Find which client sent the data
                    if (FD_ISSET (cl[i], &read_fd_set)) {

                        // 8. READ CLIENT MESSAGE
                        memset (buf,'\0',100); //clear buffer
                        ret = read(cl[i],buf,BUF_LEN);//Read the data from that client

                        // 9. PROCESS CLIENT MESSAGES
                        //---------------------------------------------------------
                            if (ret > 0) {

                                // 9.1 CLIENT MESSAGE "READY"
                                if (strncmp(buf, "ready", 5) == 0) { //compare first 5 char for 'ready'
                                    cout<<"server: read(sock:"<<cl[i]<<", buf:"<<buf<<")"<<endl;
                                    // server send message "monitor" to client
                                    memset (buf,'\0',100); //clear buffer
                                    len = sprintf(buf, "monitor")+1;
                                    ret = write(cl[i], buf, len);
                                }

                                // 9.2 CLIENT MESSAGE "LINK DOWN"
                                if (strncmp(buf, "link", 4) == 0) { //compare first 4 char for 'link'
                                    sleep(1);
                                    system("clear");
                                    cout<<"server: read(sock:"<<cl[i]<<", buf:"<<buf<<")"<<endl;
                                    // server send message "set link up" to client
                                    memset (buf,'\0',100); //clear buffer
                                    len = sprintf(buf, "set link up")+1;
                                    ret = write(cl[i], buf, len);
                                }

                                if (strncmp(buf, "Inte", 4) == 0) { //compare first 4 char for 'interface'
                                    cout << buf << endl << endl;
                                }
                            } 
                            else // read message fails
                            {
                                cout<<"server: Read Error"<<endl;
                                cout<<strerror(errno)<<endl;
                            }
                        //---------------------------------------------------------

                    }
                } // end of for loop to find which client send the message
            } // end of 7. receive message from connected client
        }
    } // end of while
    //---------------------------------------------------------------------------------

    //---------------------------------------------------------------------------------
    // 10. QUIT CLIENTS
    for(int i=0; i<numClients; ++i) {//Request each client to quit
        cout<<"server: request client "<<i+1<<" to quit"<<endl;
        len = sprintf(buf, "Quit")+1;
        ret = write(cl[i], buf, len);
        if(ret==-1) {
            cout<<"server: Write Error"<<endl;
            cout<<strerror(errno)<<endl;
        }
	    sleep(1);//Give the clients a change to quit
        FD_CLR(cl[i], &active_fd_set);//Remove the socket from the set of active sockets
        close(cl[i]);//Close the socket connection
    }
    //----------------------------------------------------------------------------------

    //Close the master socket
    close(master_fd);
    //Remove the socket file from /tmp
    unlink(socket_path);
    return 0;
}
//---------------------------------------------------