// lab 6 client - Andre Rosa 
// 07 MAR 2020
#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
   
using namespace std;

char socket_path[] = "/tmp/lab6";

int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_un addr; 
    int fd, rc;
    char buffer[100]; 
    char message[30]; 
    char charpid[6];  

    memset(&addr, 0, sizeof(addr));
    // 1. CREATE SOCKET
    if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        cout << "client1: " << strerror(errno) << endl;
        exit(-1);
    }
   
    // 2. SET SOCKET TO FILE
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    cout << "client1: addr.sun_path: " << addr.sun_path << endl;

    // 3. CONNECT TO LOCAL SOCKET
    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cout << "client error: " << strerror(errno) << endl;
        close(fd);
        exit(-1);
    }

    memset (buffer,'\0',100); //clear buffer

    // 4. SET INFINITE LOOP TO LISTEN SERVER
    bool isRunning = true;

    while (isRunning == true) {
    // 5. READ MESSAGE FROM SERVER
        rc = read( fd, buffer, 100); 
        cout << "srv msg: " << buffer << endl;

        // 5.1 REPLY TO PID REQUEST
        if (strncmp(buffer, "pid", 3) == 0) {//checks if server asked for pid
            sprintf(charpid, "%d", getpid());
            strcpy(message, "This client has pid: ");
            strcat(message, charpid);
            send(fd, message, strlen(message), 0 ); 
            memset (buffer,'\0',100); //clear buffer
        }

        // 5.2 REPLY TO SLEEP REQUEST
        if (strncmp(buffer, "sle", 3) == 0) {//checks if server asked for sleep
            sleep (5);
            strcpy(message, "done sleeping");
            send(fd, message, strlen(message), 0 ); 
            memset (buffer,'\0',100); //clear buffer
        }

        // 5.3 REPLY TO QUIT REQUEST
        if (strncmp(buffer, "qui", 3) == 0) {//checks if server asked for quit
            cout << "Quit!" << endl;
            isRunning = false;
        }
    } 
    close(fd);
    return 0; 
} 