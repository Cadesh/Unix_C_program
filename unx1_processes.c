//------------------------------------------------------------------------
// ANDRE ROSA in 11 JAN 2020
// THIS PROGRAM SEACHS FOR PROCESSES WITH MEMORY USAGE HIGHER THAN 10000k 
// AND PRINTS INFO ABOUT THEM
// PROCESSES ARE STORED UNDER /proc FOLDER AND DATA CAN BE ACCESSED
// IN THE status FILE UNDER EACH PROCESS
// compile: gcc <name.c> -o <name>
// run: ./<name>
//------------------------------------------------------------------------

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h> 

int
main (void)
{
  DIR *directory;
  FILE *fptr; 
  struct dirent *folder;
  char c;
  int procVmRss; 
  int procVmSize;
  char procName[256];
  char fpath[256]; // path for the proc/status file
  int procId;

  directory = opendir ("/proc"); // 1. open dir
  if (directory != NULL){
      while (folder = readdir (directory)) { // 2. read dir
        strcpy(fpath, "/proc/");        
        strcat(fpath, folder->d_name);
        strcat(fpath, "/status");
        //printf("%s\n", "-----------------------");
        //printf("%s\n", fpath);

        //3. load each File
        char buffer[1024] = "";
        FILE* file = fopen(fpath, "r");
        if (file != NULL) {

             // 4. read file searching for words
             while (fscanf(file, " %1023s", buffer) == 1) {

                 if (strcmp(buffer, "Name:") == 0) {;
                     fscanf(file, " %s", procName);
                 }
                 if (strcmp(buffer, "Pid:") == 0) {;
                     fscanf(file, " %d", &procId);
                 }
                 if (strcmp(buffer, "VmSize:") == 0) {;
                     fscanf(file, " %d", &procVmSize);
                 }
                 if (strcmp(buffer, "VmRSS:") == 0) {;
                     fscanf(file, " %d", &procVmRss);
                 }
             } // end of 4.

             if (procVmRss > 10000) { // print process with
                printf("%s\n", "-----------------------");
                printf("%s\n", fpath);
                printf("   PROCESS NAME:  %s\n", procName);
                printf("   PROCESS ID  :  %d\n", procId);
                printf("   VIRTUAL MEM :  %d\n", procVmSize);
                printf("   RESIDENT MEM:  %d\n", procVmRss);
             }
        } // end of 3.
      } // end of 2.
      (void) closedir (directory);
  } // end of 1. 

  return 0;
}