// Copyright 2012 CrossControl

#ifndef CANWORKERTHREAD_H
#define CANWORKERTHREAD_H

#include "CanWrapper.h"
#include <string.h>
class CanWorkerThread
{

public:

	void Init(CanWrapper *wrapper); // Initialize

    void run(int n, bool extended, bool rtr, int errorCode);         // start thread

    void shutDown();    // Make thread shut down



private:
    CanWrapper *m_can;  // Pointer to can wrapper class
    bool m_running;     // Set to false to stop thread



};

#endif // CANWORKERTHREAD_H
