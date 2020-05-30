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

    bool Init(const char *interfaceName, int &errorCode);

    void Close();

    bool SendMsg(struct can_frame msg, bool extended, bool rtr, int &errorCode);

    bool GetMsg(struct can_frame &frame, bool &extended, bool &rtr, bool &error, int &errorCode, struct timeval timeout);

    bool SetRecvBufferSize(int size);

    void EnableErrorMessages();

private:
    bool m_initialized; // indicates if socket is initialized

    int m_socket;       // Socket

};

#endif /* CANWRAPPER_H_ */
