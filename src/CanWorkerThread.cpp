// Copyright 2012 CrossControl
#include "CanWorkerThread.h"
#include "CanWrapper.h"
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <cstdio>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

// Setup worker thread
// Parameter:
// wrapper - pointer to CAN wrapper instance

void CanWorkerThread::Init()
{	printf("Starting CANthread.....");
    m_running = true;
    //CanWrapper m_can;

}

// This function will be excuted in an own thread when start is called from parent thread
// It will wait for data from CAN (blocking) and signal to GUI when data arrives
void CanWorkerThread::run(int n, bool extended, bool rtr, int errorCode)
{	CanWrapper can_m;
	int retval;
	int recvbytes = 0;
	int read_can_port;
	struct can_frame frame;
	struct timeval tv;
	bool error;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    	read_can_port = 1;
    	while (read_can_port<n) {
    		recvbytes = can_m.GetMsg(frame, extended, rtr, error, errorCode, tv);
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
    }

// Make thread to stop. Because the thread might be waiting on a blocking call, the call must be unblocked first to actually make the
// thread stop
void CanWorkerThread::shutDown()
{	printf("Shutting down the thread \n");
    m_running = false;
}

