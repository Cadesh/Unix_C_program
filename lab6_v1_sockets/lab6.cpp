#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

using namespace std;

char socket_path[] = "/tmp/lab6";

int main(int argc, char const *argv[]) 
{ 
    int server_fd, client, valread; 
    struct sockaddr_un address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[100]; 
       
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
        perror("accept"); 
        exit(-1); 
    } 

    // 6. READ MESSAGE FROM CLIENT
    valread = read( client, buffer, sizeof(buffer)); 
    printf("%s\n",buffer ); 

    // 7. SEND MESSAGE TO CLIENT
    char *message = "Hello from server"; 
    send( client, message, strlen(message) , 0 ); 
    printf("message sent\n"); 

    return 0; 
} 