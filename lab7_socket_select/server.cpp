// LAB 7 - server.cpp - Server code for multiple connections
// Author: Andre Rosa 

// ANSWERS
// 1. The Standard Output (stdout) 
// 2. Standard Input (stdin) to read the client arrival

#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>

using namespace std;

int PORT=1153;
const char IP_ADDR[]="127.0.0.1";
char socket_path[]="/tmp/sel";
const int BUF_LEN=4096;
const int MAX_CLIENTS=3;
bool is_running;

//----------------------------------------------------------
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
//-----------------------------------------------------------

//-----------------------------------------------------------
int main(int argc, char *argv[])
{
    //Create the socket for inter-process communications
    struct sockaddr_in myaddr;
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

    // CAPTURE PORT FROM ARGUMENTS
    if (argc == 2 ) {
        PORT = atoi(argv[1]);
    }

    //-------------------------------------------
    //INITIALIZE CLIENT SOCKETS TO 0
    for (int i = 0; i < MAX_CLIENTS; i++) 
    {
        cl[i] = 0;
    }
    //------------------------------------------      

    //------------------------------------------
    // CREATE SOCKET
    //------------------------------------------
    master_fd=socket(AF_INET, SOCK_STREAM, 0);
    if(master_fd<0) {
        cout<<"Cannot create the socket"<<endl;
	    cout<<strerror(errno)<<endl;
	    return -1;
    }
    //------------------------------------------

    memset(&myaddr, 0, sizeof(myaddr));
    // type of socket
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = INADDR_ANY;
    myaddr.sin_port = htons(PORT);

    //------------------------------------------
    // CONNECT
    //------------------------------------------
    ret = inet_pton(AF_INET, IP_ADDR, &myaddr.sin_addr);
    if(ret==0) {
        cout<<"No such address"<<endl;
	    cout<<strerror(errno)<<endl;
        close(master_fd);
        return -1;
    }
    //------------------------------------------

    //------------------------------------------
    // BIND
    //------------------------------------------
    ret = bind(master_fd, (struct sockaddr *)&myaddr, sizeof(myaddr));
    if(ret<0) {
       cout<<"Cannot bind the socket to the local address"<<endl;
	   cout<<strerror(errno)<<endl;
	   return -1;
    } else {
       cout<<"socket fd:"<<master_fd<<" bound to address "<<inet_ntoa(myaddr.sin_addr)<<endl;
    }
    //-------------------------------------------

    //-------------------------------------------
    // LISTEN
    //-------------------------------------------
    cout<<"Waiting for the client..."<<endl;
    //Listen for a client to connect to this local socket file
    if (listen(master_fd, MAX_CLIENTS) == -1) {
        cout << "server: " << strerror(errno) << endl;
        unlink(socket_path);
        close(master_fd);
        exit(-1);
    }
    //--------------------------------------------


    FD_ZERO(&read_fd_set);
    FD_ZERO(&active_fd_set);
    //Add the master_fd to the socket set
    FD_SET(master_fd, &active_fd_set);
    max_fd = master_fd;//We will select from max_fd+1 sockets (plus one is due to a new connection)
   
    is_running=true;
    int count = 0;
    while(is_running) {

        //Block until an input arrives on one or more sockets
        read_fd_set = active_fd_set;
        ret=select(max_fd+1, &read_fd_set, NULL, NULL, NULL);//Select from up to max_fd+1 sockets
        if (ret < 0) {
            cout << "server: " << strerror(errno) << endl;
        }
        else
        {//Service all the sockets with input pending

            //----------------------------------------------------
            // NEW CONNECTION TO MASTER
            if (FD_ISSET (master_fd, &read_fd_set))
            {
                cl[numClients] = accept(master_fd, NULL, NULL);//Accept the new connection
                if (cl[numClients] < 0) {
                    cout << "server: " << strerror(errno) << endl;
                } else {
                    cout<<"Server: incoming connection "<<cl[numClients]<<endl;
                    FD_SET (cl[numClients], &active_fd_set);//Add the new connection to the set
                    //Request the pid of the new connection
                    len = sprintf(buf, "Send Text")+1;
                    ret = write(cl[numClients], buf, len);
                    if(ret==-1) {
                        cout<<"server: Write Error"<<endl;
                        cout<<strerror(errno)<<endl;
                    }
                    if(max_fd<cl[numClients]) max_fd=cl[numClients];//Update the maximum fd
                    ++numClients;



                }
            }
            // DATA ARRIVE FROM OLD CONNECTION
            else
            {
                for (int i = 0; i < numClients; ++i) {//Find which client sent the data
                    if (FD_ISSET (cl[i], &read_fd_set)) {
                        ret = read(cl[i],buf,BUF_LEN);//Read the data from that client
                        if(ret==-1) {
                            cout<<"server: Read Error"<<endl;
                            cout<<strerror(errno)<<endl;
                        }
                        cout<<"server: read(sock:"<<cl[i]<<", buf:"<<buf<<")"<<endl;
                        count += 1; // client transmitted
                    }
                }
            }
            //-----------------------------------------------------

            if (count >=3) { // added a counter to end the loop when all clients are done 
                is_running = false;
            }

        }
    }

    for(int i=0; i<numClients; ++i) {//Request each client to quit
        cout<<"server: request client "<<i+1<<" to quit"<<endl;
        len = sprintf(buf, "Quit")+1;
        ret = write(cl[i], buf, len);
        if(ret==-1) {
            cout<<"server: Write Error"<<endl;
            cout<<strerror(errno)<<endl;
        }
	sleep(1);//Give the clients time to quit
        FD_CLR(cl[i], &active_fd_set);//Remove the socket from the set of active sockets
        close(cl[i]);//Close the socket connection
    }

    //Close the master socket
    close(master_fd);
    //Remove the socket file from /tmp
    unlink(socket_path);
    return 0;
}
