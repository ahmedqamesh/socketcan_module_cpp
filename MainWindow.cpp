/* A simple SocketCAN example */
/* root -l MainWindow.cpp */
// g++ -std=c++11 MainWindow.cpp -o mainWindow
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
int soc;
struct can_frame frame;
struct sockaddr_can addr; // has an interface index like the  PF_PACKET socket, that also binds to a specific interface:
struct ifreq ifr;
#include <iostream>
#include <cstdio>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

int main(void) {
	CanWrapper* can = new CanWrapper();
	CanWorkerThread* thread = new CanWorkerThread();
	int errorCode;
	int retval;
	int recvbytes = 0;
	int read_can_port;
	struct can_frame frame;
	struct timeval tv;

	bool extended;
	bool rtr;
	bool error;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	frame.can_dlc = 8; // Set data length
	frame.can_id = 0x601; // Set id
	// Set data elements
	frame.data[0] = 64;
	frame.data[1] = 0;
	frame.data[2] = 16;
	frame.data[3] = 0;
	frame.data[4] = 0;
	frame.data[5] = 0;

	can->Init("can0", errorCode);
	// Send CAN message on socket CAN
	retval = can->SendMsg(frame, extended, rtr, errorCode);

	// If send fails, show error dialog
	if (!retval) printf("Could not send CAN message. Error code:%d\n", errorCode);
	//read messages from the thread
	thread->Init(can);
	thread->run(3, extended, rtr, errorCode);
	thread->shutDown();
return 0;
}

