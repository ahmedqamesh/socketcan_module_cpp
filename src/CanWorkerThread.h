/*
 * CanWrapper.h
 *
 *  Created on: May 26, 2020
 *      Author: dcs
 */
#ifndef CANWORKERTHREAD_H
#define CANWORKERTHREAD_H

#include "CanWrapper.h"
#include <string.h>
class CanWorkerThread
{

public:

	void init(CanWrapper *wrapper); // Initialize

    void run(int n,  bool extended, bool rtr_frame, int errorCode);         // start thread

    void shutDown();    // Make thread shut down



private:
    CanWrapper *m_can;  // Pointer to can wrapper class
    bool m_running;     // Set to false to stop thread



};

#endif // CANWORKERTHREAD_H
