/*
 * CanWrapper.h
 *
 *  Created on: May 26, 2020
 *      Author: dcs
 */
#ifndef CANWRAPPER_H_
#define CANWRAPPER_H_
#include <string.h>
#include <tuple>
#include <vector>
#include <string>
class CanWrapper
{
public:
    CanWrapper();

    int sdoData;

    bool openPort(const char *interfaceName, int &errorCode);
    void closePort();
    bool writeCanMessage(int cobid, int msg [], int dlc, bool extended, bool rtr_frame, int &errorCode, struct timeval timeout);
    std::tuple<int, std::vector<int>, int> canMsgQueue(int codid, std::vector<int> msg, int dlc);
    bool readCanMessages(bool &extended, bool &rtr_frame, bool &error, int &errorCode, struct timeval timeout);
    bool sdoRead( int nodeId, int index, int subindex,struct timeval timeout, int dlc);
    bool setRecvBufferSize(int size);
    void enableErrorMessages();
    std::string intToHexString(int intValue);
    // Setter
     void setSdoData(int d) {
    	sdoData = d;
       }
       // Getter
       int getSdoData() {
         return sdoData;
       }


private:
    bool m_initialized; // indicates if socket is initialized
    int m_socket;       // Socket
    std::tuple<int, std::vector<int>, int> GeneratedMessage;
    bool messageValid;
    bool gotMessage;
};

#endif /* CANWRAPPER_H_ */
