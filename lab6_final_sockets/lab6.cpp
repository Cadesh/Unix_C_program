// lab 6 server - Andre Rosa 
// 07 MAR 2020
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

using namespace std;

char socket_path[] = "/tmp/lab6";

//-------------------------------------------
void sendMessage (int client, string message) {
    char lmsg [message.size()];
    memset(lmsg, 0, sizeof lmsg);
    strcpy(lmsg, message.c_str());
    send(client, lmsg, strlen(lmsg) , 0 ); 
}
//-------------------------------------------

//-------------------------------------------
int main(int argc, char const *argv[]) 
{ 
    int server_fd, client, msgval; 
    struct sockaddr_un address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[100]; 
    char message[30]; 
       
    // 1. CREATE SOCKET
    if ( (server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        cout << "server: " << strerror(errno) << endl;
        exit(-1);
    }

    // 2. SET SOCKET TO FILE
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socket_path, sizeof(address.sun_path)-1);
    cout << "server: addr.sun_path:" << address.sun_path << endl;
    unlink(socket_path);

    // 3. BINDING
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
        perror("error: bind failed"); 
        exit(-1); 
    } 
    // 4. LISTENING FOR CLIENT
    if (listen(server_fd, 3) < 0)  { 
        perror("error: listen fails"); 
        exit(-1); 
    } 
    // 5. ACCEPT CLIENT
    if ((client = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) { 
        perror("error: accept client"); 
        exit(-1); 
    } else {
        cout << "client connected" << endl;
    }

    memset (buffer,'\0',100); //clear buffer

    // 7. COMMUNICATION SCRIPT
    //---------------------------------------------------------
    // 7.1.1 SEND MESSAGE ASKING FOR PID
    sendMessage(client, "pid");
    bool askQUIT = false;
    bool askPID = true;
    // 7.1.2 WAITS FOR PID REPPLY
    while (askPID == true) {
        msgval = read( client, buffer, sizeof(buffer)); 
        if (msgval > 0) {
            if (strncmp(buffer, "This client has pid", 15) == 0) { //compare first 3 chard for 'pid'
                cout << buffer << endl; // print pid message 
                askPID = false;
            }
        }
    } // end of askPID
    //---------------------------------------------------------

    memset (buffer,'\0',100); //clear buffer

    //---------------------------------------------------------
    // 7.2.1 SEND MESSAGE ASKING TO SLEEP
    sendMessage(client, "sleep");
    bool askSLEEP = true;
    // 7.2.2 WAITS FOR SLEEP REPPLY
    while (askSLEEP == true) {
        msgval = read( client, buffer, sizeof(buffer)); 
        if (msgval > 0) {
            cout << buffer << endl; // print "done" message 
            askSLEEP = false;
            askQUIT = true;
        }
    } // end of askSLEEP
    //---------------------------------------------------------

    sleep (2);

    //---------------------------------------------------------
    // 7.3 SEND MESSAGE ASKING TO QUIT
    if (askQUIT == true) {
        sendMessage(client, "quit");
    }
    //---------------------------------------------------------

    cout << "server: close(server_fd), close(client)" << endl;
    unlink(socket_path);
    close(server_fd);
    close(client);
    return 0;
} 