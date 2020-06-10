/*
 * CanWrapper.h
 *
 *  Created on: May 26, 2020
 *      Author: dcs
 */
#ifndef CANWRAPPER_H_
#define CANWRAPPER_H_
#include <string.h>

class CanWrapper
{
public:
    CanWrapper();

    bool openPort(const char *interfaceName, int &errorCode);

    void closePort();

    bool writeCanMessage(int cobid, int msg [], int dlc, bool extended, bool rtr_frame, int &errorCode);

    bool readCanMessages(bool &extended, bool &rtr, bool &error, int &errorCode, struct timeval timeout);
    bool sdoRead( int nodeId, int index, int subindex,struct timeval timeout, int dlc);
    bool setRecvBufferSize(int size);

    void enableErrorMessages();

private:
    bool m_initialized; // indicates if socket is initialized

    int m_socket;       // Socket

};

#endif /* CANWRAPPER_H_ */
