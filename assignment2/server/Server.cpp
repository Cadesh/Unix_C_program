// ASSIGNMENT 2 - server.cpp
// Author: Andre Rosa 

// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <iostream>
#include <fstream>  
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <queue>

using namespace std;

#define PORT	9000 
#define MAXLINE 1024 

pthread_mutex_t lock_x;
pthread_t tid;
int sockfd; 
char buffer[MAXLINE]; 
struct sockaddr_in servaddr, cliaddr; 
bool is_running = false;

//--------------------------------------------------------------
// RUN THREAD - receives logs from client
//--------------------------------------------------------------
void *runThread(void *arg)
{
	int n; 
    int fd = *((int *) arg);
    int count=0;
	socklen_t len = sizeof(cliaddr); 
    int fOut = open ("log.txt", O_WRONLY|O_CREAT|O_TRUNC, 0666);
	while (is_running == true) {
		// RECEIVE MESSAGE
		pthread_mutex_lock(&lock_x); // LOCK
		n = recvfrom(sockfd, (char *)buffer, MAXLINE,  MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len); 
		pthread_mutex_unlock(&lock_x);
		buffer[n] = '\0'; 
		//printf("Client : %s\n", buffer); 
		write(fOut,buffer,strlen(buffer));
	}
	close(fOut);
	pthread_exit(NULL);
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// SEND TO CLIENT NEW LOG LEVEL MESSAGE
//--------------------------------------------------------------
void sendNewLogLevelMsg (int i) {
	string str="Set Log Level=";
	str += to_string(i);
	const char *msg = str.c_str();//"Set Log Level=3";
	socklen_t len = sizeof(cliaddr); 
	sendto(sockfd, (const char *)msg, strlen(msg), 
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// EXIT SERVER - close socket fd, stop thread
//--------------------------------------------------------------
void ExitServer() {
    sleep (1); // give time for thread to stop
    // 1. stop thread, destroy mutex
    pthread_join(tid, NULL);
	pthread_exit(NULL);
    pthread_mutex_destroy(&lock_x);
    // 2. close file descriptor 
    close(sockfd); 
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// PRINT TO SCREEN
//--------------------------------------------------------------
void printLogToScreen() {
  ifstream in("log.txt");
  string line;
  system("clear");
  while(getline(in, line)) 
  {
    cout << line << endl;
  }
  cout << endl << endl;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// MAIN
//--------------------------------------------------------------
int main() { 
	// 1. Creating socket UDP
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// 2. Fill server information 
	servaddr.sin_family = AF_INET; // IPv4 
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// 3. Bind socket 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	// 4. Initialize Thread Mutex
    pthread_mutex_init(&lock_x, NULL);

    // 5. Start the receive thread and pass fd
	is_running = true;
    int fd = sockfd;
    int ret = pthread_create(&tid, NULL, runThread, &fd);
    if(ret!=0) {
        cout<<"Cannot create thread"<<endl;
        cout<<strerror(errno)<<endl;
        return -1;
    }

	int choice;
	bool runMenu = true;
	while (runMenu)
	{
		cout << " 1 - Set Log Level.\n";
		cout << " 2 - Show Log.\n";
		cout << " 0 - Shut Down.\n";
		cout << " Enter your choice and press return: ";
		cin >> choice;

		switch (choice)
		{	
		case 1: {
			char loglvl = 0;
			cout << " Select log level.\n";
			cout << " 0 - Debug.\n";
			cout << " 1 - Warning.\n";
			cout << " 2 - Error.\n";
			cout << " 3 - Critical.\n";
			cin >> loglvl;
			int lvl = (loglvl - '0');
			if ((lvl >=0) && (lvl <=3)) {
				sendNewLogLevelMsg(lvl);
			}
			break;
		}
		case 2:
			cout << "show log on screen\n";
			printLogToScreen();
			break;
		case 0: //EXIT
			is_running = false;
			runMenu = false;
			cout << "exiting..." << endl;
			break;
		default:
			cout << "Invalid choice, try again.\n";
			cin >> choice;
			break;
		}
	} 
	ExitServer();
	return 0; 
} 
//--------------------------------------------------------------
