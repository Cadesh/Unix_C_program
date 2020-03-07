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

    // 4. SEND MESSAGE TO SERVER
    char *message = "Hello from client"; 
    send(fd, message , strlen(message) , 0 ); 
    printf("message sent\n"); 


    // 5. READ MESSAGE FROM SERVER
    rc = read( fd, buffer, 100); 
    printf("%s\n",buffer ); 
    return 0; 

} 