#pragma once

//-------------------------------------------------------------------------------------------------------
// Copyright 2014 DGLogik Inc. All rights reserved.
// Licensed under the Apache License, Version 2.0 (the "License"). 
// See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "app-config.h"
#include "monitor-libuv.h"

#include <iostream>
#include <functional>
#include <memory>

using namespace std;

/** The class TransporterTCP owns a platform specific IO Monitoring class. */
template<class T>
class TransporterTCP
{

public:
	TransporterTCP(){}
	virtual ~TransporterTCP(){}
	
	void enableTCPClient() {
		tcpIOMonitor(UV_TCP);
		tcpIOMonitor.establishConnection();
	}

	// sets up an events monitoring thread
	void run() {
		tcpIOMonitor.run();
	}
private:
	UVMonitor tcpIOMonitor;
};

