/* A simple SocketCAN example */
/* root -l main.cpp */

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

void MainWindow(void) {
	CanWrapper can;
	CanWorkerThread thread;
	int errorCode;
	int retval;
	int elements;
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
	frame.can_dlc = 8;
	frame.data[0] = 64;
	frame.data[1] = 0;
	frame.data[2] = 16;
	frame.data[3] = 0;
	frame.data[4] = 0;
	frame.data[5] = 0;

	can.Init("can0", errorCode);
	thread.Init();
	// Send CAN message on socket CAN
	retval = can.SendMsg(frame, extended, rtr, errorCode);

	// If send fails, show error dialog
	if (!retval) printf("Could not send CAN message. Error code:%d\n", errorCode);
	thread.run(10, extended, rtr, errorCode);
	//thread.shutDown();
	read_can_port = 1;
	while (read_can_port<2) {
		recvbytes = can.GetMsg(frame, extended, rtr, error, errorCode, tv);
		if (recvbytes) {
			if (error)   // Error frame
			{
				printf("Error frame received, class = %d\n", frame.can_id);
			} else if (extended)   // Extended frame
			{
				printf("Extended Frame msg........\n");
				if (rtr) {
					printf("RTR ID: %d LENGTH: %d\n", frame.can_id,
							frame.can_dlc);
				} else {
					printf("ID: %d LENGTH: %d  DATA:\n", frame.can_id,
							frame.can_dlc);
					for (int i = 0; i <= frame.can_dlc; i++) {
						printf(" DATA[%i]:%i\n",i, frame.data[i]);
					}
				}
			} else    // Standard frame
			{
				printf("Standard Frame msg........\n");
				if (rtr) {
					printf("RTR ID: %d LENGTH: %d\n", frame.can_id,
							frame.can_dlc);
				} else {
					printf("ID: %d LENGTH: %d \n", frame.can_id,
							frame.can_dlc);
					for (int i = 0; i <= frame.can_dlc; i++) {
						printf(" DATA[%i]:%i\n",i, frame.data[i]);
					}


				}
			}
		}
	read_can_port++;
	}
	can.Close();

}

