//ifMonitor.cpp - process child
//
// Andre Rosa
//

// The Interface Monitor
// Each interface will have a process dedicated to it. This is called the interface monitor. 
// For instance, if there are three network interfaces on a Linux machine, 
// three instances of the interface monitor process will be running. Each interface monitor will:
//ok     • Communicate with the network monitor through a temporary socket file in the /tmp directory.
//ok     • Inform the network monitor when it is ready to start monitoring. 
//ok     • Monitor and print out the statistics for a given interface as per instructed by the network monitor. (printMonitor function)
//ok    • Report to the network monitor if its interface does down. (inside print monitor function 1.3)
//todo     • Be able to set an interface link up as per instructed by the network monitor. (setLinkUp function)
//ok     • Perform a controlled shutdown if it receives a ctrl-C of if instructed by the network monitor by closing the socket connection
//       to the network monitor then exiting. (5.3 msg quit in main function)

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

//--------------------------------------------------------------------
// PRINT NETWORK INTERFACE DATA
// @param {string} iName - the interface name
//--------------------------------------------------------------------
void printMonitor(string iName, int fd)
{
    // 1. SET LOOP FOR MONITORING
    bool isMonitoring=true;
    while (isMonitoring == true) {
        // 1.1 SET VARIABLES
        // line 1
        string path = "/sys/class/net/" + iName + "/";
        string operstateVal;
        string up_countVal; 
        string down_countVal;
        // line 2
        string rx_bytes;
        string rx_dropped;
        string rx_errors;
        string rx_packets;
        // line 3
        string tx_bytes;
        string tx_dropped;
        string tx_errors;
        string tx_packets;

        // 1.2 OPEN FILES TO RETRIEVE DATA TO VARIABLES
        string operstatePath = path + "operstate";
        ifstream infile(operstatePath.c_str());
        if(infile.is_open()) {
            while(!infile.eof()) {
               infile >> operstateVal;  
            }
            infile.close();
        }
        string up_countPath = path + "carrier_up_count"; 
        ifstream infile2(up_countPath.c_str());
        if(infile2.is_open()) {
            while(!infile2.eof()) {
               infile2 >> up_countVal;  
            }
            infile2.close();
        }
        string down_countPath = path + "carrier_down_count"; 
        ifstream infile3(down_countPath.c_str());
        if(infile3.is_open()) {
            while(!infile3.eof()) {
               infile3 >> down_countVal;  
            }
            infile3.close();
        }
        // line 2
        string rx_bytesPath = path + "statistics/rx_bytes";
        ifstream infile4(rx_bytesPath.c_str());
        if(infile4.is_open()) {
            while(!infile4.eof()) {
               infile4 >> rx_bytes;  
            }
            infile4.close();
        }
        string rx_droppedPath = path + "statistics/rx_dropped";
        ifstream infile5(rx_droppedPath.c_str());
        if(infile5.is_open()) {
            while(!infile5.eof()) {
               infile5 >> rx_dropped;  
            }
            infile5.close();
        }
        string rx_errorsPath = path + "statistics/rx_errors";
        ifstream infile6(rx_errorsPath.c_str());
        if(infile6.is_open()) {
            while(!infile6.eof()) {
               infile6 >> rx_errors;  
            }
            infile6.close();
        }
        string rx_packetsPath = path + "statistics/rx_packets";
        ifstream infile7(rx_packetsPath.c_str());
        if(infile7.is_open()) {
            while(!infile7.eof()) {
               infile7 >> rx_packets;  
            }
            infile7.close();
        }
        //line 3
        string tx_bytesPath = path + "statistics/tx_bytes";
        ifstream infile8(tx_bytesPath.c_str());
        if(infile8.is_open()) {
            while(!infile8.eof()) {
               infile8 >> tx_bytes;  
            }
            infile8.close();
        }
        string tx_droppedPath = path + "statistics/tx_dropped";
        ifstream infile9(tx_droppedPath.c_str());
        if(infile9.is_open()) {
            while(!infile9.eof()) {
               infile9 >> tx_dropped;  
            }
            infile9.close();
        }
        string tx_errorsPath = path + "statistics/tx_errors";
        ifstream infile10(tx_errorsPath.c_str());
        if(infile10.is_open()) {
            while(!infile10.eof()) {
               infile10 >> tx_errors;  
            }
            infile10.close();
        }
        string tx_packetsPath = path + "statistics/tx_packets";
        ifstream infile11(tx_packetsPath.c_str());
        if(infile11.is_open()) {
            while(!infile11.eof()) {
               infile11 >> tx_packets;  
            }
            infile11.close();
        }

        // 1.3 CHECK IF THE INTERFACE IS DOWN
        if (operstateVal.compare("up") != 0) { // if it is not 'up'
            isMonitoring = false; // exit loop
            send(fd, "link down", strlen("link down"), 0 ); //alert server that interface is down
        }

        // 1.4 PRINT OUTPUT
        cout<< "Interface:" << iName << " state:" << operstateVal << " up_count:" <<up_countVal << 
        " down_count:" << down_countVal << endl;
        cout << " rx_bytes:" << rx_bytes << " rx_dropped:" << rx_dropped << " rx_errors:" << rx_errors << 
        " rx_packets:" << rx_packets << endl;
        cout << " tx_bytes:" << tx_bytes << " tx_dropped:" << tx_dropped << " tx_errors:" << tx_errors << 
        " tx_packets:" << tx_packets << endl;
        sleep(1);
    } // END OF LOOP 1.
}
//---------------------------------------------------------------

//--------------------------------------------------------------------
// PRINT NETWORK INTERFACE DATA
// @param {string} iName - the interface name
// Uses IOCTL to set up the interface, if fails print an error
//--------------------------------------------------------------------
void setLinkUp(string iName) {
//TO DO

}


//---------------------------------------------------------
// MAIN
// @argument 1 - name of interface monitor
//---------------------------------------------------------
int main(int argc, char const *argv[]) 
{ 
    struct sockaddr_un addr; 
    int fd, rc;
    char buffer[100]; 
    char message[30]; 
    char charpid[6];  
    bool isRunning=true;
    int begin;
    int end;
    string iName = "";
    int retVal=0;

    if(argc<2) {
        cout<<"iMonitor: Incorrect number of parameters"<<endl;
	isRunning=false;
	retVal=-1;
    } else {
        iName = argv[1]; // saves name of the interface
        cout<<"iMonitor: interface name:"<< iName << endl;
    }

    char socket_path[] = "/tmp/"; // set the path to the socket file
    strcat(socket_path, iName.c_str()); // use a file with the same name of the interface

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
    } else {  // 3.1 tells the server it is ready to run
        sleep (5);
        strcpy(message, "ready");
        send(fd, message, strlen(message), 0 ); 
    }

    memset (buffer,'\0',100); //clear buffer

    //----------------------------------------
    // 4. SET INFINITE LOOP TO LISTEN SERVER
    while (isRunning == true) {

        // 5. READ MESSAGE FROM SERVER
        rc = read( fd, buffer, 100); 
        cout << "srv msg: " << buffer << endl;

        // 5.2 START MONITOR AND PRINT STATISTICS
        if (strncmp(buffer, "mon", 3) == 0) {//monitor
            sleep (1);
            printMonitor(iName, fd); // calls function to print interface data
            memset (buffer,'\0',100); //clear buffer
        }

        // 5.2 SET LINK UP MESSAGE
        if (strncmp(buffer, "set", 3) == 0) {//monitor sends 'set link up'
            sleep (1);
            setLinkUp(iName); // calls function to print interface data
            memset (buffer,'\0',100); //clear buffer
        }

        // 5.2 REPLY TO SLEEP REQUEST
        // if (strncmp(buffer, "sle", 3) == 0) {//checks if server asked for sleep
        //     sleep (5);
        //     strcpy(message, "done sleeping");
        //     send(fd, message, strlen(message), 0 ); 
        //     memset (buffer,'\0',100); //clear buffer
        // }

        // 5.3 REPLY TO QUIT REQUEST
        if (strncmp(buffer, "qui", 3) == 0) {//checks if server asked for quit
            cout << "Quit!" << endl;
            isRunning = false;
        }
    } // end of while (isRunning == true) 
    //----------------------------------------
    close(fd);
    return 0; 
} 

//--------------------------------------------------------------------
