// ASSIGNMENT 1 - netMonitor.cpp - Server code for multiple connections
// Authors: Andre Valle-Rosa /Yathavan Parameshwaran

// TODO LIST:
// SERVER netMonitor
// 1 - Number 0. below (capture arguments with interface names)
// 2 - Number 5. below (start children with fork, right now I am activating children manually)
// ifMonitor
// 1 - Test Quit/Shutdown from server
// 2 - Test reactivation of interface (see function setLinkUp)
// 3 - Todo client "Done" message (see documentation page 4)


#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>

using namespace std;

char socket_path[]="/tmp/master";
const int BUF_LEN=100;

const int MAX_CLIENTS=5;
bool is_running;

// SIGNAL HANDLER TO QUIT CODE
//-----------------------------------------------
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
//------------------------------------------------

int main()
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

    // 0. GET ARGUMENTS WITH INTERFACE NAMES 
    //----------------------------
    // TODO
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
    // 5. START CLIENTS WITH FORK
    // right now it is necessary to start clients manually with command ./ifMonitor ens33
    // TODO
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
                        ret = read(cl[i],buf,BUF_LEN);//Read the data from that client

                        // 9. PROCESS CLIENT MESSAGES
                        //---------------------------------------------------------
                            if (ret > 0) {

                                // 9.1 CLIENT MESSAGE "READY"
                                if (strncmp(buf, "ready", 5) == 0) { //compare first 5 char for 'ready'
                                    cout<<"server: read(sock:"<<cl[i]<<", buf:"<<buf<<")"<<endl;
                                    // server send message "monitor" to client
                                    len = sprintf(buf, "monitor")+1;
                                    ret = write(cl[i], buf, len);
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