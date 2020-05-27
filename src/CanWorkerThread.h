// Copyright 2012 CrossControl

#ifndef CANWORKERTHREAD_H
#define CANWORKERTHREAD_H


#include "CanWrapper.h"
#include <string.h>
class CanWorkerThread
{

public:

    void Init(); // Initialize

    void run(int n, bool extended, bool rtr, int errorCode);         // start thread

    void shutDown();    // Make thread shut down



private:

    bool m_running;     // Set to false to stop thread



};

#endif // CANWORKERTHREAD_H
