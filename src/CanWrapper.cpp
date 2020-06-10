/*
 * CanWrapper.cpp
 *
 *  Created on: May 26, 2020
 *      Author: dcs
 */

#include <string.h>
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
#include "CanWrapper.h"

#define INVALID_SOCKET -1

CanWrapper::CanWrapper() {
	m_initialized = false;
	m_socket = INVALID_SOCKET;
}
// Initialize socket. Returns false if socket could not be opened.
// Parameters:
// interfaceName - the name of the CAN interface to open (can0, can1...)
// errorCode - error code indicating why init did fail
bool CanWrapper::openPort(const char *interfaceName, int &errorCode) {
	struct sockaddr_can addr; // has an interface index like the  PF_PACKET socket, that also binds to a specific interface:
	struct ifreq ifr;

	int ret;

	errorCode = 0;
	printf(
			"Opening an empty socket API socket with PF_CAN as the protocol family\n");
	m_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

	// Get index for a certain name
	strcpy(ifr.ifr_name, interfaceName);
	ret = ioctl(m_socket, SIOCGIFINDEX, &ifr);
	if (ret) {
		errorCode = errno;

		return false;
	}
	printf("Binding the socket %s to all CAN interfaces\n", interfaceName);
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	ret = bind(m_socket, (struct sockaddr *) &addr, sizeof(addr));
	if (ret) {
		errorCode = errno;

		close(m_socket);
		m_socket = INVALID_SOCKET;

		return false;
	}

	m_initialized = true;

	return true;
}

// Close an open connection. Use when changing can net.
void CanWrapper::closePort() {
	printf("Closing the connection \n");
	if (m_initialized) {
		// We don't want to read or write anything more
		shutdown(m_socket, SHUT_RDWR);

		close(m_socket);

		m_socket = INVALID_SOCKET;

		m_initialized = false;
	}
}

// Send a message on the CAN-bus. Returns true if ok, false if not. Additional error info can be found in errorCode.
// Parameters:
// msg - the can message to send
// extended - set to true to send an extended frame
// rtr - set to true to send a remot request (rtr)
// errorcode - will be set to an error code
// Common errors:
//#define ENETDOWN        100     /* Network is down - use ifconfig up to start */
//#define EAGAIN          11      /* Try again - buffer is full */
//#define EBADF            9      /* Bad file number - can net not opened */
bool CanWrapper::writeCanMessage(int cobid, int msg [], int dlc, bool extended, bool rtr_frame,
		int &errorCode) {
	printf("Writing CAN frames.... \n");
	struct can_frame frame;
	frame.can_dlc = dlc; // Set data length
	frame.can_id = cobid; // Set id
	// Set data elements
	frame.data[0] = msg[0];
	frame.data[1] = msg[1];
	frame.data[2] = msg[2];
	frame.data[3] = msg[3];
	frame.data[4] = msg[4];
	frame.data[5] = msg[5];
	int retval;
	struct ifreq ifr;
	errorCode = 0;

	if (!m_initialized)
		return false;

	if (extended) {
		frame.can_id |= CAN_EFF_FLAG;
	}

	if (rtr_frame) {
		frame.can_id |= CAN_RTR_FLAG;
	}
	printf("Received a CAN frame from interface %s\n", ifr.ifr_name);
	retval = write(m_socket, &frame, sizeof(struct can_frame));

	if (retval < 0) {
		perror("could not send");
		printf("errno is %d\r\n", errno);
		errorCode = errno;

		return false;
	}

	return true;
}
// Get a CAN message. If socket is blocking (default) - this call will block until data is received or until timeout period has expired.
// If socket is non blocking, it will return false if there is no data or if there is any error.
// If socket is blocking, it will return false if there is an error or at timeout.
// Parameters:
// frame - the received can frame
// extended - will be set to true if the received frame was an extended frame
// rtr - will be set to true if the received frame was an rtr frame
// errorcode - error code
// Common errors:
//#define ENETDOWN        100     /* Network is down - use ifconfig up to start*/
//#define EAGAIN          11      /* Try again - no data available*/
//#define EBADF            9      /* Bad file number - can net not opened */
// timeout - GetMsg will return false after timeout period
bool CanWrapper::readCanMessages(bool &extended, bool &rtr,
		bool &error, int &errorCode, struct timeval timeout) {
	printf("Reading CAN frames.... \n");
	struct can_frame frame;
	int bytesRead;
	int ret;
	fd_set rfds;

	errorCode = 0;

	if (!m_initialized) {
		return false;
	}

	// Set up a file descriptor set only containing one socket
	FD_ZERO(&rfds);
	FD_SET(m_socket, &rfds);

	// Use select to be able to use a timeout
	ret = select(m_socket + 1, &rfds, NULL, NULL, &timeout);
	if (ret < 0) {
		errorCode = errno;

		return false;
	}

	if (ret > 0) {
		bytesRead = read(m_socket, &frame, sizeof(frame));

		if (bytesRead < 0) {
			errorCode = errno;

			return false;
		}

		if (bytesRead == sizeof(frame)) {
			error = frame.can_id & CAN_ERR_FLAG;

			extended = frame.can_id & CAN_EFF_FLAG;

			rtr = frame.can_id & CAN_RTR_FLAG;
			if (error) {
				frame.can_id &= CAN_ERR_MASK;
			}

			if (extended) {
				frame.can_id &= CAN_EFF_MASK;
			} else {
				frame.can_id &= CAN_SFF_MASK;
			}

			return true;
		}
	}

	return false;
}
bool CanWrapper::sdoRead( int nodeId, int index, int subindex,struct timeval timeout, int dlc){
	/*
	  Read an object via |SDO|

			Currently expedited and segmented transfer is supported by this method.
			The function will writing the dictionary request from the master to the node then read the response from the node to the master
			The user has to decide how to decode the data.

			Parameters
			----------
			nodeId : :obj:`int`
				The id from the node to read from
			index : :obj:`int`
				The Object Dictionary index to read from
			subindex : :obj:`int`
				|OD| Subindex. Defaults to zero for single value entries.
			timeout : :obj:`int`, optional
				|SDO| timeout in milliseconds

			Returns
			-------
			:obj:`list` of :obj:`int`
				The data if was successfully read
			:data:`None`
				In case of errors
	*/
	printf("Reading an object via |SDO|\n");
	int errorCode;
	bool extended ;
	extended = false;
	bool rtr_frame = false;
	int SDO_TX = 0x580;
    int SDO_RX = 0x600;
    int Byte0, Byte1, Byte2, Byte3;
    if (sizeof(index)<=0|| sizeof(subindex)<=0){
    	printf("SDO read protocol cancelled before it could begin\n");
             return false;
		};
    int cobid =  SDO_RX + nodeId;
    int msg[dlc];
    msg[0] = Byte0 = 0x40;
    msg[1] = static_cast<unsigned char>((index & 0x00FF));
    msg[2] = static_cast<unsigned char>(((index & 0xFF00) >> 8));
    msg[3] = subindex;
    CanWrapper::writeCanMessage(cobid,msg,dlc, extended, rtr_frame, errorCode);
	//Wait for response
    bool messageValid;

    return true;
}

// Set size of receive buffer. The standard size is usually large enough.
// Note that getsockopt will return twice the size set
// If settings a larger size than the system supports, the size will set to a lower value than requested
// Parameters:
// size - the requested size of the receive buffer
bool CanWrapper::setRecvBufferSize(int size) {
	int ret;
	int rcvbuf_size = size;

//    int rbuf;
//    int len = sizeof(int);

// Print receive buf size before change
//    ret = getsockopt(m_socket,SOL_SOCKET,SO_RCVBUF,&rbuf,(socklen_t*)&len);
//    printf("receive buf size is before change: %d\r\n", rbuf);

	ret = setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size,
			sizeof(rcvbuf_size));
	if (ret < 0) {
		//      perror("set recv buffer size");
		//      printf("errno is %d\r\n", errno);

		return false;
	}

	// Print receive buf size after change
//    ret = getsockopt(m_socket,SOL_SOCKET,SO_RCVBUF,&rbuf,(socklen_t*)&len);
//    printf("receive buf size is after change: %d\r\n", rbuf);

	return true;
}

// Configure the socket can layer to report errors
// see /linux/can/error.h for more options regading error handling (not implemented in this class).

void CanWrapper::enableErrorMessages() {
	int ret;

	can_err_mask_t err_mask = ( CAN_ERR_TX_TIMEOUT | CAN_ERR_BUSOFF);

	ret = setsockopt(m_socket, SOL_CAN_RAW, CAN_RAW_ERR_FILTER, &err_mask,
			sizeof(err_mask));
	if (ret > 0) {
		printf("errno is %d\r\n", ret);

	}
}
