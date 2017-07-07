#pragma once
//-------------------------------------------------------------------------------------------------------
// Copyright 2014 DGLogik Inc. All rights reserved.
// Licensed under the Apache License, Version 2.0 (the "License"). 
// See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "defs-libuv.h"
#include "app-config.h"

struct UVMonitor 
{
	UVMonitor();
	virtual ~UVMonitor();

	// create a TCP Socket resource; 
	void operator()(int stream_type);

	void establishConnection();

	int run();
private:
	int	start_events_monitor(int stream_type);
	sockaddr_in make_ipendpoint();
	void make_dataflow_connection(sockaddr_in& dest);

private:
	UVMessage_unique_ptr rcMessageSocket;
	UVConnect_unique_ptr rcFlowLive;
	UVEventsLoop_unique_ptr eventsLoop;
};