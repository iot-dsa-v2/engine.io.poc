#pragma once

//-------------------------------------------------------------------------------------------------------
// Copyright 2014 DGLogik Inc. All rights reserved.
// Licensed under the Apache License, Version 2.0 (the "License"). 
// See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include <uv.h>
#include "uv_msg_framing.h"
#include "uv_send_message.h"
#include <iostream>
#include <functional>
#include <memory>
#include <system_error>

// forward declaration
struct UVMonitor;

using namespace std;

//-------------------------------------------------------------------------------------------------------
// declares smart pointers types and reference wrapper types for life cycle management.
//-------------------------------------------------------------------------------------------------------

using UVMonitor_unique_ptr = std::unique_ptr<UVMonitor>;			// declares a smart pointer type for UVMonitor

using UVMessage_unique_ptr = std::unique_ptr<uv_msg_t>;				// declares a smart pointer to manage uv_msg_t resource
using UVMessage_ref = reference_wrapper<UVMessage_unique_ptr>;		// provides a reference to the handle

using UVConnect_unique_ptr = std::unique_ptr<uv_connect_t>;			// declares a smart pointer type to manage uv_connect_t resource
using UVConnect_ref = reference_wrapper<UVConnect_unique_ptr>;		// provides a reference to the handle

using UVEventsLoop_unique_ptr = std::unique_ptr<uv_loop_t>;				// declares a smart pointer to manage uv_loop_t resource
using UVLoop_ref = reference_wrapper<UVEventsLoop_unique_ptr>;		// provides a reference to the handle

using fnConnect2Server = uv_connect_cb;								// declares a type that assigns a lambda to c-style function pointer

//-------------------------------------------------------------------------------------------------------
// factory methods and functors that manage libuv resources.
//-------------------------------------------------------------------------------------------------------
inline UVMonitor_unique_ptr make_UVMonitor_unique_ptr()
{
	UVMonitor_unique_ptr uvMon = make_unique<UVMonitor>();

	return (std::move(uvMon));
}

inline UVMessage_unique_ptr make_uvmsg_unique_ptr()
{
	UVMessage_unique_ptr msgSocket = make_unique<uv_msg_t>();

	return (std::move(msgSocket));
}