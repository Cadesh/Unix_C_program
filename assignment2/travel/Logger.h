#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <time.h>
#include <iostream>

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <queue>

using namespace std;

//--------------------------------------------------------------
// GLOBALS
//--------------------------------------------------------------
extern const char* SERVER_IP;
extern const int PORT; 
//extern const int BUF_LEN; 
const int BUF_LEN = 1024; 

enum LOG_LEVEL { DEBUG = 0, WARNING = 1, ERROR = 2, CRITICAL = 3};
extern const char levelStr[][16];

extern int _LogLevel;
extern bool is_running;

extern struct sockaddr_in servaddr; 
extern int sockfd; 
extern char buffer[BUF_LEN]; 
extern pthread_mutex_t lock_x;
extern pthread_t tid;
extern queue<string> message;
//--------------------------------------------------------------

//--------------------------------------------------------------
// FUNCTIONS
//--------------------------------------------------------------
int InitializeLog();
void ExitLog();
void SetLogLevel (LOG_LEVEL level);
void *runThread(void *fd);
void Log(LOG_LEVEL level,
        const char* prog, 
        const char* func, 
        int line, 
        const char* message);
LOG_LEVEL convertIntToLogLevel(int a);
//--------------------------------------------------------------