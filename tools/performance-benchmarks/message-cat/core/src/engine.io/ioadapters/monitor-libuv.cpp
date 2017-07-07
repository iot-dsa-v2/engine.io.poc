//-------------------------------------------------------------------------------------------------------
// Copyright 2014 DGLogik Inc. All rights reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
// See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "monitor-libuv.h"
#include "v2error.h"
#include "payload-generator.h"
#include "timestamp.h"
#include "fnlambdas-libuv.h"

UVMonitor::UVMonitor()
{
	rcMessageSocket = make_uvmsg_unique_ptr();
	eventsLoop.reset(uv_default_loop());
};

UVMonitor::~UVMonitor()
{
	cout << "~UVMonitor() destructor" << endl;
};

// create a TCP Socket resource; 
void UVMonitor::operator()(int stream_type) 
{
	int ec;
	ec = start_events_monitor(stream_type);

	// throws when not 0 or configure to log the error.
	if (ec) {
		std::error_code reason = std::make_error_code(std::errc::connection_aborted); // works
		dsav2::error().operator()(reason);
	}
}

void UVMonitor::establishConnection()
{
	// setup server address to use
	struct sockaddr_in dest;
	dest = make_ipendpoint();

	make_dataflow_connection(dest);
}

int UVMonitor::run() 
{
	return uv_run(eventsLoop.get(), UV_RUN_DEFAULT);
};

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

/* rc (resource codes shld be converted to ec (error codes) and returned.*/
int  UVMonitor::start_events_monitor(int stream_type)
{
	int rc;	
	rc = uv_msg_init(eventsLoop.get(), rcMessageSocket.get(), stream_type);

	return rc;
}

/*makes an IP endpoint based on appConfig*/
sockaddr_in UVMonitor::make_ipendpoint() 
{
	struct sockaddr_in dest;
	uv_ip4_addr(get_appconfig().get().ipAdress().c_str(), 
				get_appconfig().get().defaultPort(), &dest);

	return std::move(dest);
}

void  UVMonitor::make_dataflow_connection(sockaddr_in& dest)
{
	rcFlowLive = make_unique<uv_connect_t>();
	
	int rc;

	/* rcFlowLive is released to the C library here. consider using shared_ptr*/
	rc = uv_tcp_connect(std::move(rcFlowLive.get()), (uv_tcp_t*)rcMessageSocket.get(),
		(const struct sockaddr*)&dest, fnOnFlowEstablished);
}