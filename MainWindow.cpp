/* A simple SocketCAN example */
/* root -l MainWindow.cpp */
// g++ -std=c++11 -o mainWindow MainWindow.cpp[or clang++ -o mainWindow MainWindow.cpp] [clang++ -std=c++11 -o mainWindow MainWindow.cpp]
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
#include <iostream>
#include <cstdio>
#include<stdio.h>
#include<stdlib.h>

#include <iomanip>
using namespace std;
int main(void) {
	CanWrapper* can = new CanWrapper();
	int errorCode;
	bool extended ;
	bool error;
	extended = false;
	bool rtr_frame = false;
	bool output= false;

	//Set data elements
	int SDO_RX = 0x600;
	int nodeId =1;
    int index = 0x1000;
    int  subindex,Byte0, Byte1, Byte2, Byte3;
    struct timeval timeout;
    int dlc = 8;// Set data length
    Byte0 = 0x40;
    Byte1 = static_cast<unsigned char>((index & 0x00FF));
	Byte2 = static_cast<unsigned char>(((index & 0xFF00) >> 8));
	Byte3 = subindex = 0;
	int msg[] = {Byte0,Byte1,Byte2,Byte3,0,0,0,0};
	int cobid = SDO_RX +1;// Set id
	int retval;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	can->openPort("can0", errorCode);
	// Send CAN message on socket CAN
	//retval = can->writeCanMessage(cobid,msg,dlc, extended, rtr_frame, errorCode,timeout);
	 //If send fails, show error dialog
	//if (!retval) printf("Could not send CAN message. Error code:%d\n", errorCode);

	//write sdo message
    printf("Writing example CAN Expedited read message ...\n");
    //Example (1): get node Id
    std::stringstream stream;
     while (true){
		 output = can->sdoRead(nodeId,index,subindex,timeout,dlc);
		if (output){
			int return_value = can->getSdoData();
		//std::string return_value = can->intToHexString(can->getSdoData());
		//stream >> std::hex >> return_value;
		std::cout<<"VendorId: "<<return_value<< '\n';}

     }

	can->closePort();
return 0;
}

