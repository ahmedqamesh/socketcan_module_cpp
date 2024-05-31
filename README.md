
# C++ SocketCAN 
This is a simple script demonstrating how to communicate with a CAN device using the SocketCAN interface in C++. This code is a part of a larger system designed to read CAN messages from an ASIC chip and transmit the data via an OPCUA server.


## Overview
The provided code establishes communication with a CAN device and sends a Service Data Object (SDO) request to retrieve data from the device. It then waits for a response and prints the returned data.

## Prerequisites
- Linux-based system with SocketCAN support
- CAN device connected to the system (e.g., CAN bus adapter)
- Basic understanding of CAN communication protocols

## Installation
1. Clone this repository to your local machine:
'''
git clone https://github.com/example/socketcan-cpp-example.git
'''
2. Compile the code using a C++ compiler with C++11 support:
'''
g++ -std=c++11 -o main main.cpp CanWrapper.cpp
'''
**Note:** The following Twiki might be useful https://linuxhint.com/install_llvm_centos7/

## Usage
1. Run the compiled executable:
'''
./main
'''
2. The program will attempt to read data from the CAN device and print the retrieved data to the console.

## Configuration
- Adjust the CAN interface name (`"can0"`) in the code to match your system's configuration if necessary.
- Modify the node ID, index, and subindex parameters to suit your specific CAN device and data requirements.

## Contributing and Contact Information:
We welcome contributions from the community please contact : ahmed.qamesh@gmail.com.
