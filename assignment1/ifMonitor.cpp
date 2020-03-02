//ifMonitor.cpp - process child
//
// Andre Rosa
//
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>

using namespace std;

//char procDir[] = "/sys/class/net";

int main(int argc, char *argv[])
{
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
        iName = argv[1];
        //begin = atoi(argv[1]);
        //end = atoi(argv[2]);
        cout<<"iMonitor: interface name:"<< iName << endl;
    }
    while(isRunning) {

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

        cout<< "Interface:" << iName << " state:" << operstateVal << " up_count:" <<up_countVal << 
        " down_count:" << down_countVal << endl;
        cout << " rx_bytes:" << rx_bytes << " rx_dropped:" << rx_dropped << " rx_errors:" << rx_errors << 
        " rx_packets:" << rx_packets << endl;
        cout << " tx_bytes:" << tx_bytes << " tx_dropped:" << tx_dropped << " tx_errors:" << tx_errors << 
        " tx_packets:" << tx_packets << endl;
        
	sleep(1);
    }

    return retVal;
}