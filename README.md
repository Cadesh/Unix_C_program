# VMWARE Setup

Download and install VMWare Workstation Player (Windows 64-bit):
[https://my.vmware.com/en/web/vmware/free#desktop_end_user_computing/vmware_workstation_player/15_0]

Download Ubuntu Desktop ISO file (choose Ubuntu 18.04.1 LTS) and store it on your file system:
[https://www.ubuntu.com/download/desktop]

## Install C/C++ Compilers

You may have to perform another update: 
>sudo apt update

Install gcc:
>sudo apt install gcc

Install g++: 
>sudo apt install g++

Install the vim editor: 
>sudo apt install vim

Install the make facility: 
>sudo apt install make

Install ifconfig for network configuration: 
>sudo apt install net-tools

Install the ssh server: 
>sudo apt install openssh-server

## Compiling 

For .c files:
>gcc <name.c> -o <name>

For .cpp files:
>g++ -I -Wall -o <name> <name.cpp>