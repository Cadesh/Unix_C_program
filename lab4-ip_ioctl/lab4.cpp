#include <errno.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <unistd.h>

#define NAME_SIZE 16

using namespace std;

int main()
{
    int fd;
    int ret;
    int selection;
    struct ifreq ifr;
    char if_name[NAME_SIZE];
    unsigned char *mac=NULL;
    unsigned char *ip=NULL;

    // 1. List interface names
    cout << "Available interfaces:\n";
    system("ls /sys/class/net");

    cout << "Enter the interface name: ";
    cin >> if_name;

    // 2. Create ifreq structure for pass data in/out of ioctl
    size_t if_name_len=strlen(if_name);
    if (if_name_len<sizeof(ifr.ifr_name)) {
        memcpy(ifr.ifr_name,if_name,if_name_len);
        ifr.ifr_name[if_name_len]=0;
    } else {
        cout << "Interface name is too long!" << endl;
	return -1;
    }

    // 3. Provide open socket descriptor
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd<0) {
        cout << strerror(errno);
	return -1;
    }

    system("clear");
    do {
    cout << "Choose from the following:" << endl;
	cout << "1. Hardware address" << endl;
	cout << "2. IP address" << endl;
	cout << "3. Network mask" << endl;
	cout << "4. Broadcast address" << endl;
	cout << "0. Exit" << endl << endl;
	cin >> selection;
	switch(selection) {
        case 1: // 4. Hardware sddress

            // 4.1 Invoke IOCTL
            ret = ioctl(fd, SIOCGIFHWADDR, &ifr);
            if(ret<0) {
                cout << strerror(errno);
            // 4.2 Check the type of returned hardware address
            } else if(ifr.ifr_hwaddr.sa_family!=ARPHRD_ETHER) {
                cout << "not an Ethernet interface" << endl;
            } else { // 4.3 Extract MAC address from ifreq structure 
                mac=(unsigned char*)ifr.ifr_hwaddr.sa_data;
                printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	        }

            break;
        case 2: // 5. IP address

            // 5.1 Invoke IOCTL
            ret = ioctl(fd, SIOCGIFADDR, &ifr);
            if(ret<0) { // test IOCTL for error
                cout << strerror(errno);
            // 5.2 Extract IP address from ifreq structure
            } else {
               struct sockaddr_in* ip = (struct sockaddr_in*)&ifr.ifr_addr;
               printf("IP address: %s\n",inet_ntoa(ip->sin_addr)); 
            }

            break;
        case 3: // 6. Network mask

            // 6.1 Invoke IOCTL
            ret = ioctl(fd, SIOCGIFNETMASK, &ifr);
            if(ret<0) { // test IOCTL for error
                cout << strerror(errno);
            // 6.2 Extract IP address from ifreq structure
            } else {
               struct sockaddr_in* mask = (struct sockaddr_in*)&ifr.ifr_addr;
               printf("Network mask: %s\n",inet_ntoa(mask->sin_addr)); 
            }  

            break;
        case 4: // 7. Broadcast

            // 7.1 Invoke IOCTL
            ret = ioctl(fd, SIOCGIFBRDADDR, &ifr);
            if(ret<0) { // test IOCTL for error
                cout << strerror(errno);
            // 7.2 Extract IP address from ifreq structure
            } else {
               struct sockaddr_in* mask = (struct sockaddr_in*)&ifr.ifr_addr;
               printf("Broadcast Address: %s\n",inet_ntoa(mask->sin_addr)); 
            }  
            
            break;
        }
	if(selection!=0) {
            char key;
            cout << "Press any key to continue: ";
            cin >> key;
            system("clear");
        }
    } while (selection!=0);


    close(fd); 
    return 0;
}