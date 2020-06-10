/* A simple SocketCAN example */
/* root -l MainWindow.cpp */
// g++ -std=c++11 -o mainWindow MainWindow.cpp[or clang++ -o mainWindow MainWindow.cpp]
// ./mainWindow
#include <stdio.h>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/can.h> // includes The basic CAN frame structure and the sockaddr structure
#include <linux/can/raw.h>
#include "src/CanWrapper.cpp"
#include "src/CanWorkerThread.cpp"
#include <iostream>
#include <cstdio>
#include<stdio.h>
#include<stdlib.h>
using namespace std;
int main(void) {
	CanWrapper* can = new CanWrapper();
	CanWorkerThread* thread = new CanWorkerThread();
	int errorCode;
	bool extended ;
	extended = false;
	bool rtr_frame = false;
	//Set data elements
	int SDO_RX = 0x600;
	int nodeid = 1;
    int index = 0x1000;
    int  subindex,Byte0, Byte1, Byte2, Byte3;
    Byte0 = 0x40;
    Byte1 = static_cast<unsigned char>((index & 0x00FF));
	Byte2 = static_cast<unsigned char>(((index & 0xFF00) >> 8));
	Byte3 = subindex = 0;

	int msg[] = {Byte0,Byte1,Byte2,Byte3,0,0};
	//int msg[] = {64,0,16,0,0,0};
	int dlc = 8;// Set data length
	int cobid = SDO_RX +1;// Set id
	int retval;
	struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
	can->openPort("can0", errorCode);

	// Send CAN message on socket CAN
	//retval = can->writeCanMessage(cobid,msg,dlc, extended, rtr_frame, errorCode);
	// If send fails, show error dialog
	//if (!retval) printf("Could not send CAN message. Error code:%d\n", errorCode);
	can->sdoRead(nodeid,index,subindex,tv,dlc);
	//read messages from the thread
	//thread->init(can);
	//thread->run(10, extended, rtr_frame, errorCode);
	//thread->shutDown();
return 0;
}

