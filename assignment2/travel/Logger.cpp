
#include "Logger.h"

const char* SERVER_IP = "127.0.0.1";
const int PORT = 9000; 
//const int BUF_LEN = 1024; 
const char levelStr[][16]={"DEBUG", "WARNING", "ERROR", "CRITICAL"};
int _LogLevel = LOG_LEVEL::DEBUG;
bool is_running = true;

struct sockaddr_in servaddr; 
int sockfd; 
pthread_mutex_t lock_x;
pthread_t tid;
char buffer[BUF_LEN]; 

//--------------------------------------------------------------
// INITIALIZE LOGGER - Create socket, set adresss, initialize Mutex, star receive thread
//--------------------------------------------------------------
int InitializeLog() {
    is_running = true;

    // 1. Create a socket for UDP 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 

    // 2. Set address and port of server
	servaddr.sin_family = AF_INET; 
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY;

    // 3. Initialize Thread Mutex
    pthread_mutex_init(&lock_x, NULL);

    // 4. Start the receive thread and pass fd
    int fd = sockfd;
    int ret = pthread_create(&tid, NULL, runThread, &fd);
    if(ret!=0) {
        cout<<"Cannot create thread"<<endl;
        cout<<strerror(errno)<<endl;
        return -1;
    }

    return 0;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// RUN THREAD - receives set log level from server
//--------------------------------------------------------------
void *runThread(void *arg)
{
    int fd = *((int *) arg);
    cout<<"starting thread"<<endl;
    while(is_running) {
        int n; 
        socklen_t len = sizeof(servaddr);
        //pthread_mutex_lock(&lock_x); // LOCK
        memset(buffer, 0, BUF_LEN);
        n = recvfrom(sockfd, (char *)buffer, BUF_LEN, 
                    0, (struct sockaddr *) &servaddr, 
                    &len); 
        buffer[n] = '\0'; 
        int ia = buffer[n-1] - '0';
        SetLogLevel(convertIntToLogLevel(ia));
        printf("Server set log level: %c\n", buffer[n-1]); 

        //pthread_mutex_unlock(&lock_x); // UNLOCK
    }
    pthread_exit(NULL);
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// LOG - send log message to server
//--------------------------------------------------------------
void Log(LOG_LEVEL level, const char* file, const char* func, int line, const char* message) {
    if (is_running == true) {
        if (level >= _LogLevel) { // compare severity for log


            // 1. Build Message
            time_t now = time(0);
            char *dt = ctime(&now);
            pthread_mutex_lock(&lock_x); // LOCK
            memset(buffer, 0, BUF_LEN);
            int len = sprintf(buffer, "%s %s %s:%s:%d %s\n", dt, levelStr[level], file, func, line, message)+1;
            buffer[len-1]='\0';
            pthread_mutex_unlock(&lock_x); // UNLOCK

            // 2. Send Message
            sendto(sockfd, buffer, len, 
                MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
                    sizeof(servaddr)); 
            //cout << "Log message sent" << endl; 
        }
    }
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// EXIT LOGGER - close socket fd, stop thread
//--------------------------------------------------------------
void ExitLog() {
    is_running = false;
    sleep (1); // give time for thread to stop
    // 1. stop thread, destroy mutex
    pthread_join(tid, NULL);
    pthread_mutex_destroy(&lock_x);
    // 2. close file descriptor 
    close(sockfd); 
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// SET LOG LEVEL - used when server requests a log level change
//--------------------------------------------------------------
void SetLogLevel (LOG_LEVEL level) { 
    _LogLevel = level;
    cout << "log level set to " << _LogLevel << endl;
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// CONVERT INT TO LOG LEVEL
//--------------------------------------------------------------
LOG_LEVEL convertIntToLogLevel(int a){

    switch(a) {
        case 0 : return DEBUG;    break;
        case 1 : return WARNING;  break;
        case 2 : return ERROR;    break;
        case 3 : return CRITICAL; break;
        default: return DEBUG;
    }
}
//--------------------------------------------------------------

//--------------------------------------------------------------
// MAIN - used for testing only
//--------------------------------------------------------------
// int main () {
//     //Logger logger;
//     InitializeLog();
//     SetLogLevel(WARNING);
//     Log(DEBUG, __FILE__, __func__, __LINE__, "Debug message test");
    
//     while (is_running == true) {
//         Log(ERROR, __FILE__, __func__, __LINE__, "Error message test");
//         sleep(2);
//     }
// }
//--------------------------------------------------------------