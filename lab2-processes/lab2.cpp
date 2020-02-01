// UNX511 lab2
// author: Andre Rosa
// compile: g++ -I -Wall -o <appName> <fileName.cpp> -L. <libraries.a>
// example: g++ -I -Wall -o Lab2 lab2.cpp -L. -lPidUtil
// ATTENTION the -L. tells the compiler the libray is in the same folder of the .cpp code

#include "pidUtil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (void) {

ErrStatus errorStatus;
string errorMsg = " ";
string name = "no name";
vector<int> pidList;

//-------------------------------
// 1. Print all PIDs and Names
errorStatus = GetAllPids(pidList);
if (errorStatus == 0) {
    for ( auto i: pidList ) {
        errorStatus = GetNameByPid(i, name);
        if (errorStatus == 0) {
            printf("%d %s\n", i, name.c_str()); 
        } else {
            errorMsg =  GetErrorMsg(errorStatus);  
        }
    }
} else {
    errorMsg =  GetErrorMsg(errorStatus);      
}
//-------------------------------

//-------------------------------
// 2. Set PID to 1 and print PID name
int lPID = 1;
errorStatus = GetNameByPid(lPID, name);
if (errorStatus == 0) {
    printf("The name of pid of %d is %s\n", lPID, name.c_str()); 
} else {
    errorMsg =  GetErrorMsg(errorStatus);        
}

//-------------------------------

//-------------------------------
//3. Set name to lab2 and print PID
name = "Lab2";
errorStatus = GetPidByName(name, lPID);
if (errorStatus == 0) {
    printf("The pid of %s is %d\n", name.c_str(), lPID);
} else {
    errorMsg =  GetErrorMsg(errorStatus);    
}
//------------------------------

//-----------------------------
//4. Set an invalid name get an error
name = "Lab22";
errorStatus = GetPidByName(name, lPID);
if (errorStatus == 0) {
    printf("The pid of %s is %d\n", name.c_str(), lPID);
} else {
    errorMsg =  GetErrorMsg(errorStatus);
    printf("No such name\n");
}
//----------------------------

return 0;
}