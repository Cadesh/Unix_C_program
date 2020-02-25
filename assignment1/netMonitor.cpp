#include <errno.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

#include <ifaddrs.h>
#include <sys/types.h>
#include <sys/socket.h>

#define NAME_SIZE 16

using namespace std;

//---------------------------------------------------
// RETURN A LIST OF INTERFACE NAMES (SPACE SEPARATED)
string getInterfacesList() {
  struct ifaddrs *ifaddr, *ifa;
  string ifList = "";

  if (getifaddrs(&ifaddr) == -1) {
    perror("getifaddrs");
    return "";
  }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    char protocol[IFNAMSIZ]  = {0};

    if (ifa->ifa_addr == NULL ||
        ifa->ifa_addr->sa_family != AF_PACKET) continue;

    ifList += ifa->ifa_name;

    if (ifa->ifa_next != NULL) 
        ifList += " ";
    //printf("%s ", ifa->ifa_name);

  }
  freeifaddrs(ifaddr);
  return ifList;
}
//---------------------------------------------------

//---------------------------------------------------
// CREATE AN ARRAY OF INTERFACE NAMES
vector <string> processNames (string interfaceNamesStr) {
    vector<string> lInterfaceNames;
    string word = ""; 
    for (auto x : interfaceNamesStr)
    { 
        if (x == ' ') 
        { 
            lInterfaceNames.push_back(word);
            word = ""; 
        } 
        else
        { 
            word = word + x; 
        } 
    }  
  return lInterfaceNames;
}
//---------------------------------------------------

//---------------------------------------------------
// PRINT ARRAY (TO TEST ARRAY OF INTERFACES)
void printArray (vector <string> pStrs) {

   for (int count = 0; count < pStrs.size(); count++)   
       cout << pStrs[count] << endl;
  
   return;
}
//--------------------------------------------------

int main()
{
    int selection;
    int interfaceNumber = -1;
    string interfaceStr = "";
    string input = "";
    vector<string> interfaceNames;

    //system("clear");
    do {
    cout << "Choose from the following:" << endl;
    cout << "1. List available interfaces" << endl;
    cout << "2. Select interfaces to monitor" << endl;
	cout << "0. Exit" << endl << endl;
	cin >> selection;
	switch(selection) {

        case 1: // 1. List interface names
            cout << "Available interfaces:\n";
            interfaceStr = getInterfacesList();
            cout << interfaceStr << endl;
            break;

        case 2: // 2. Select interfaces to monitor
            //system ("clear");
            while (true) {
                cout << "How many interfaces do you want to monitor? (-1 for all)" << endl;
                getline(cin, input);
                stringstream mystream(input);
                if (mystream >> interfaceNumber) {
                    if (interfaceNumber == 0) {// monitor none
                       cout << "None? then exit" << endl;
                       break;
                    }
                    if (interfaceNumber == -1) {// monitor all
                        cout << "Monitor all" << endl;
                        interfaceStr = getInterfacesList();
                        interfaceNames = processNames (interfaceStr);
                        //printArray(interfaceNames);
                        // call function to monitor
                        
                        break;
                    } else { // another number?
                        cout << "Type the interface names to monitor (space separated)" << endl;
                        getline(cin, input);
                        interfaceNames = processNames (input);
                        //printArray(interfaceNames);
                        break;
                    } 
                    
                    break;
                }   
                cout << "Invalid number, please try again" << endl;  
            }
            break;

        }
	if(selection!=0) {
            char key;
            cout << "Press any key to continue: ";
            cin >> key;
            while ((getchar()) != '\n'); 
            //system("clear");
        }
    } while (selection!=0);

    return 0;
}
