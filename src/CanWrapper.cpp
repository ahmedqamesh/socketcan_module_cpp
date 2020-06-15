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
#include <typeinfo>
#include <tuple>
#include <algorithm>
#include <vector>
#include <bitset>

#define INVALID_SOCKET -1

/* Helper functions */
void errExit(char* msg) {
	perror(msg);
	exit(EXIT_FAILURE);
}

CanWrapper::CanWrapper() {
	m_initialized = false;
	gotMessage = false;
	m_socket = INVALID_SOCKET;
	std::vector<int> msg;
	GeneratedMessage = make_tuple(0, msg, 0);
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
bool CanWrapper::writeCanMessage(int cobid, int msg[], int dlc, bool extended,
		bool rtr_frame, int &errorCode, struct timeval timeout) {
	printf("Writing CAN frames.... \n");
	struct can_frame frame;
	frame.can_dlc = dlc; // Set data length
	frame.can_id = cobid; // Set id
	// Set data elements
	int retval;
	struct ifreq ifr;
	errorCode = 0;

	std::vector<int> vmsg;
	for (int i = 0; i < frame.can_dlc; i++) {
		frame.data[i] = msg[i];
		vmsg.push_back(frame.data[i]);
	}
	auto result = CanWrapper::canMsgQueue(int(frame.can_id), vmsg,
			int(frame.can_dlc));
	std::cout << "ID:" << std::hex << std::get<0>(result) << ", " << "Data: [";
	std::vector<int> msg_tuple = std::get<1>(result);
	for (std::vector<int>::iterator it = msg_tuple.begin();
			it != msg_tuple.end(); ++it)
		std::cout << ' ' << *it;
	printf("], DLC:%i\n", std::get<2>(result));

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
//std::tuple<int, std::vector<int>, int>
std::tuple<int, std::vector<int>, int> CanWrapper::canMsgQueue(int codid,
		std::vector<int> msg, int dlc) {
	return std::make_tuple(codid, msg, dlc);
}

bool CanWrapper::readCanMessages(bool &extended, bool &rtr_frame, bool &error,
		int &errorCode, struct timeval timeout) {
	printf("Reading CAN frames.... \n");
	struct can_frame frame_rd;
	int bytesRead;
	int ret;
	int dlc = frame_rd.can_dlc;

	fd_set rfds;
	errorCode = 0;

	if (!m_initialized) {
		return false; //std::make_tuple(false,false, false); //false;
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
		gotMessage = true;
		bytesRead = read(m_socket, &frame_rd, sizeof(frame_rd));
		if (bytesRead < 0) {
			errorCode = errno;
			return false;
		}

		if (bytesRead == sizeof(frame_rd)) {
			error = frame_rd.can_id & CAN_ERR_FLAG;
			extended = frame_rd.can_id & CAN_EFF_FLAG;
			rtr_frame = frame_rd.can_id & CAN_RTR_FLAG;
			if (error) { // Error frame
				frame_rd.can_id &= CAN_ERR_MASK;
				printf("Error frame received, class = %d\n", frame_rd.can_id);
			} else if (extended) { // Extended frame
				frame_rd.can_id &= CAN_EFF_MASK;
				printf("Extended Frame msg........\n");
				if (rtr_frame) {
					printf("RTR ID: %d LENGTH: %d\n", frame_rd.can_id,
							frame_rd.can_dlc);
				} else {
					printf("ID: %d LENGTH: %d  DATA:\n", frame_rd.can_id,
							frame_rd.can_dlc);
					for (int i = 0; i < frame_rd.can_dlc; i++) {
						printf(" DATA[%i]:%i\n", i, frame_rd.data[i]);
					}
				}
			} else { // Standard frame
				frame_rd.can_id &= CAN_SFF_MASK;
				printf("Standard Frame msg........\n");
				if (rtr_frame) {
					printf("RTR ID: %d LENGTH: %d\n", frame_rd.can_id,
							frame_rd.can_dlc);
				} else {
					std::vector<int> vmsg;
					for (int i = 0; i < frame_rd.can_dlc; i++) {
						vmsg.push_back(frame_rd.data[i]);
					}
					GeneratedMessage = CanWrapper::canMsgQueue(
							int(frame_rd.can_id), vmsg, int(frame_rd.can_dlc));
				}
			}
			return true;
		}
	}
	return false;
}
double CanWrapper::sdoRead(int nodeId, int index, int subindex,
		struct timeval timeout, int dlc) {
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
	int recvbytes = 0;
	int errorCode;
	bool extended;
	bool error;
	extended = false;
	bool rtr_frame = false;
	int retval;
	struct can_frame frame;
	int SDO_TX = 0x580;
	int SDO_RX = 0x600;
	int Byte0, Byte1, Byte2, Byte3;
	if (sizeof(index) <= 0 || sizeof(subindex) <= 0) {
		printf("SDO read protocol cancelled before it could begin\n");
		//return false;
	};
	double return_value;
	int cobid = SDO_RX + nodeId;
	Byte0 = 0x40;
	Byte1 = static_cast<unsigned char>((index & 0x00FF));
	Byte2 = static_cast<unsigned char>(((index & 0xFF00) >> 8));
	Byte3 = subindex;
	int msg[8] = { Byte0, Byte1, Byte2, Byte3, 0, 0, 0, 0 };
	printf("Send SDO read request to node %i\n", nodeId);
	retval = CanWrapper::writeCanMessage(cobid, msg, dlc, extended, rtr_frame, errorCode,
			timeout);
	if (!retval){
		printf("Could not send CAN message. Error code:%d\n", errorCode);
		//return false;
	}
	else{
		CanWrapper::readCanMessages(extended, rtr_frame, error, errorCode, timeout);
		messageValid = false;
		if (gotMessage) {
			int cobid_ret = std::get<0>(GeneratedMessage);
			std::vector<int> ret = std::get<1>(GeneratedMessage);
			int dlc = std::get<2>(GeneratedMessage);
			//check the message validity [nodid, msg size,...]
			int cmd []={0x80, 0x43, 0x47, 0x4b, 0x4f, 0x42};
			size_t cmdSize = sizeof(cmd) / sizeof(int);
			int *end = cmd + cmdSize;
			int *result = std::find(cmd, end, ret.at(0));
			//To make the most significant byte occupy the top bits, you need to shift its value up (by 8 bits), which is done with the left-shift bitwise operator <<.
			if (dlc == 8
					&& cobid_ret == SDO_TX + nodeId
					&& result != end
					&& (ret.at(2) << 8 | ret.at(1)) == index
					&& ret.at(3) == subindex){
				messageValid =true;
				std::bitset<8> bytes (4- (ret.at(0)>>2 & 0011));
				int nDatabytes;
				if (ret.at(0) != 0x42)nDatabytes = bytes.to_ulong();
				else nDatabytes = 4;
				std::cout <<"Got Data: [";
				for (int i = 0; i <nDatabytes; i++)std::cout <<ret.at(4 + i)<<" ";
				std::cout << "]"<<"\n";
				return_value = (((int32_t)ret.at(4 + 3)) << 24) | (((int32_t)ret.at(4 + 2)) << 16) | (((int32_t)ret.at(4 + 1)) << 8) | ((int32_t)ret.at(4 + 0));
			}else {
						messageValid =false;
						std::cout << "SDO read response timeout (node:" << nodeId <<", index:"<< std::hex<<index<<", subindex:"<< std::hex<<subindex<<")\n";
				}
		}
	}
	return return_value;
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
