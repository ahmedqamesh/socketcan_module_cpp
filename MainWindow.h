/*
 * MainWindow.h
 *
 *  Created on: May 26, 2020
 *      Author: dcs
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <src/CanWorkerThread.h>
#include <src/CanWrapper.h>

class MainWindow
{
public:


private:
		CanWrapper *can;             // Can wrapper class
	    CanWorkerThread *thread;    // Thread that blocks and listens for CAN messages
};
#endif /* MAINWINDOW_H_ */
