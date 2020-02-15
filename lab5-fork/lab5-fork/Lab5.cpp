//Lab5.cpp - process monitor parent
//
// 05-Feb-20  M. Watler         Created.
//
// Lab5.cpp student Andre Rosa 115997173
// Question7: There are no specific reason for the order of the processes to be executed
// The only sure thing is that the parent will be the last to finish because of the wait() 


#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sstream>


using namespace std;
const int NUM=4;
char procDir[] = "/proc";
const int MAXBUF=16;

int main(void)
{
    int begin, end;
    const int numProcesses=1000;
    //int childPid;
    pid_t childPid[NUM];
    bool isParent=true;

    cout<<endl<<"parent:main: pid:"<<getpid()<<endl;
    //Start 4 instances of procMonitor with begin and end. For instance:
    //    ./procMonitor   0 249
    //    ./procMonitor 250 499
    //    ./procMonitor 500 749
    //    ./procMonitor 750 999
    for(int i=0; i<NUM && isParent; ++i) {
        begin=numProcesses*i/4;
        end=numProcesses*(i+1)/4-1;

        // 1. Code to activate child processes
        childPid[i] = fork(); // activate one child
        if(childPid[i]==0) {  
            cout << "\nchild:main: pid:"<< getpid() << " - parent: pid:" << getppid() << endl;
            isParent=false;

            const char *cmd = "./procMonitor";
            char *args[3];
            args[0] = "./procMonitor";
            char *sBegin;
            sprintf(sBegin, "%d", begin);
            args[1] = sBegin;
            char *sEnd;
            sprintf(sEnd, "%d", end);
            args[2] = sEnd;

            execvp(cmd, args); // pass paramenters

            // 2. If your exec fails, print out the system error message strerror(errno)
            // execvp exits 'if', if it is here we got an error
            cout << "child:main: pid:"<<getpid()<<" could not start child!"<<endl;
	        cout<<strerror(errno)<<endl;
        }

    }
    //Enter your code here to monitor all child processes.
    //Print out a message if a child has terminated. For instance:
    //"Child with pid 123 has finished"
    for(int i=0;i<NUM;i++) {
         pid_t pp = wait(NULL); 
         cout << "\nChild with pid "<< pp <<" has finished"<<endl;
    }
    //If there are no more children running, end the program here.
    cout << "\nNo more child processes, exiting "<< getpid() <<endl;

    return 0;
}

