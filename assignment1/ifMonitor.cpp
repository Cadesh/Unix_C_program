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

        string path = "/sys/class/net/" + iName + "/";
        string operstateVal;
        string up_countVal; 
        string down_countVal;
        // string srx_bytes;
        // string srx_dropped;
        // string srx_errors;
        // string srx_packets;
        // string stx_bytes;
        // string stx_dropped;
        // string stx_errors;
        // string tx_packets;

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

        cout<< "Interface:" << iName << " state:" << operstateVal << " up_count:" <<up_countVal << 
        " down_count" << down_countVal << endl;

	sleep(1);
    }

    return retVal;
}