#pragma once
#include "app-config.h"
#include "tcp.h"

using namespace std;

/**
*
* IOProcessor class creates a DataFlow with the another "communication end point".
*
* This can be over a network interface such as an Ethernet. 
* Other IO Processors may exist that can transfer data over an IPC channel and for 
* transferring data to/fro a Storage end point.
*
* //
* //@author  Ravi M
* //@since   DSAv2
*/

struct IOProcessor
{
public:
	IOProcessor() {};
	virtual ~IOProcessor() {};

	void run() {
		transportTCP.run();
	}

	void createDataFlowClient() {
		transportTCP.enableTCPClient();
	}

private:
	TransporterTCP<UVMonitor> transportTCP;
};

