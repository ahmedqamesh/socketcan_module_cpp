/*
 * CanWrapper.h
 *
 *  Created on: May 26, 2020
 *      Author: dcs
 */
#include "CanWorkerThread.h"
#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/can.h> // includes The basic CAN frame structure and the sockaddr structure
#include <linux/can/raw.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <linux/can/error.h>
#include <sys/time.h>
#include <sys/select.h>

using namespace std;

// Setup worker thread
// Parameter:
// wrapper - pointer to CAN wrapper instance


// Setup worker thread
// Parameter:
// wrapper - pointer to CAN wrapper instance
void CanWorkerThread::init(CanWrapper *wrapper)
{
    m_running = true;
    m_can = wrapper;
}

// This function will be excuted in an own thread when start is called from parent thread
// It will wait for data from CAN (blocking) and signal to GUI when data arrives
void CanWorkerThread::run(int n,  bool extended, bool rtr_frame, int errorCode)
{
	int recvbytes = 0;
	int read_can_port;
	struct can_frame frame;
	struct timeval tv;
	bool messageValid;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    	read_can_port = 1;
    	while (read_can_port<n) {
    		recvbytes = m_can->readCanMessages(extended, rtr_frame, messageValid, errorCode, tv);
    		if (recvbytes) {
    			if (messageValid)   // Error frame
    			{
    				printf("Error frame received, class = %d\n", frame.can_id);
    			} else if (extended)   // Extended frame
    			{
    				printf("Extended Frame frame........\n");
    				if (rtr_frame) {
    					printf("rtr_frame ID: %d LENGTH: %d\n", frame.can_id,
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
    				printf("Standard Frame frame........\n");
    				if (rtr_frame) {
    					printf("rtr_frame ID: %d LENGTH: %d\n", frame.can_id,
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

