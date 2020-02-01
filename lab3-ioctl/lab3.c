// UNX511 lab3
// CONTROL A CDROM DRIVE WITH IOCTL
// author: Andre Rosa
// compile: gcc <name.c> -o <name>
// run: ./<name>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/cdrom.h>
#include <sys/ioctl.h>
#include <unistd.h>

//----------------------------------------
// RETUNS A STRING WITH THE CD STATUS DESCRIPTION
//----------------------------------------
char* getStatusDescription (const int cdStatus) {
    char *description = "";
    switch(cdStatus)
    {
        case CDS_TRAY_OPEN:
            description = "Tray is open";
        case CDS_NO_DISC:
            description = "No Disk";
        case CDS_DRIVE_NOT_READY:
            description = "Drive not ready";
        case CDS_DISC_OK:
            description = "Disk OK";
        case -1: // error
            description = "Error";
        case CDS_NO_INFO:
            description = "No Information";
        otherwise:
            description = "Unkown Status code";

    }
    return description;
}
//---------------------------------------

//---------------------------------------
// MAIN
//---------------------------------------
int main (int argc, char* argv[])
{
    int select = 5;
    while(select!=0) {
        // 1. Menu
        char menu;
        printf("\nMENU");
        printf("\n 1. Eject CD-ROM");
        printf("\n 2. Close the CD-ROM");
        printf("\n 3. Get the tray position");
        printf("\n 0. Exit");
        printf("\n Select Option: ");
        scanf("%c", &menu);
        select = menu - '0';
        printf("Option Selected: %d\n", select);
        getchar(); // wait the user type something to continue

        // 2. Access CDROM file
        char *lFile = "/dev/dvd"; // dvd is the name of the file
        int cdFile = open(lFile, O_RDONLY | O_NONBLOCK);
        // if cannot access cdrom gives an error
        if(cdFile == -1){
            printf("Failed to open '%s'\n", lFile);
            select = 0;
        }

        // 3. Process Menu Selection
        switch(select)
            {
            case 0: // 3.0 Exit program
                printf("\nExit program");
                select = 0;
                break;
            case 1: // 3.1 Eject CD-ROM
                printf("\nEject CD-ROM");
                ioctl (cdFile, CDROMEJECT);
                break;
            case 2: // 3.2 Close CD-ROM
                printf("\nClose CD-ROM");
                ioctl (cdFile, CDROMCLOSETRAY);
                break;
            case 3: // 3.3 Get tray position
                printf("\nTRAY POSITION");
                int status = ioctl (cdFile, CDROM_DRIVE_STATUS);
                char *statusDescription = getStatusDescription(status);
                printf("\nStatus: %s",statusDescription);
                break;
            otherwise:
                printf("\nInvalid Option");
                break;
            }
    close(cdFile); // close CDROM file
    printf("\n\n");
    }

return 0;
}
//------------------------------------