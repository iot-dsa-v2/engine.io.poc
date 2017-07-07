#pragma once

//-------------------------------------------------------------------------------------------------------
// Copyright 2014 DGLogik Inc. All rights reserved.
// Licensed under the Apache License, Version 2.0 (the "License"). 
// See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

/**
  * module refactor: ToDo.
  */

static int pubTotal()
{
	static const int pubCount = 1000000;
	return pubCount;
}


auto fn_new_buffer = [](uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
	buf->base = (char*) new char[suggested_size];
	buf->len = (long)suggested_size;
};

auto fn_delete_buffer = [](uv_handle_t* handle, void* ptr) 
{
	delete[] ptr;
};

auto fn_on_msg_received = [](uv_msg_t *client, void *msg, int size) 
{
	static int count = 0;
	if (size < 0) {
		if (size != UV_EOF) {
			fprintf(stderr, "Read error: %s\n", uv_err_name(size));
		}
		uv_close((uv_handle_t*)client, NULL);
		return;
	}

	//printf("new message received %d (%d bytes): %s\n", count++, size, (char*)msg);
	printf("new message received %d (%d bytes): \n", count++, size);

	//if (count % m.PubTotal() == 0) {
	if (count % 100000 == 0) {
		for (TS* t : timeStamps) {
			printf("elapsed time for %d msgs = %f\n", pubTotal(), time_end(t->tsId) / 1000);
		}
	}

	//if (count % 100000 == 0) {
	//	currentTimeSession = time_start();
	//}

	/*
	if (strcmp((const char *) msg, "Is it working?") == 0) {
	static int count = 0;
	char *response = "Yeaaah!";
	printf("Received %d %s\n", count++, (char*)msg);
	if (count % 100000 == 0) {
	currentTimeSession = time_start();
	}
	if (count % m.PubTotal() == 0) {
	for (TS* t : timeStamps) {
	printf("elapsed time for %d msgs = %f\n", m.PubTotal(), time_end(t->tsId) / 1000);
	}
	}
	//send_message(client, response, strlen(response) + 1, UV_MSG_STATIC, on_msg_sent, 0);
	}*/

};

auto fn_on_msg_sent = [](send_message_t *req, int status) 
{

	if (status < 0) {
		printf("message send failed:\n");
	}
	//else {
	//printf("message sent: %s   user_data: %d\n", (char*)req->msg, (int)req->data);
	//}

};

/* monitor calls this site on data flow establishment with remote end point*/
auto fnOnFlowEstablished = [](uv_connect_t *connect, int status) 
{
	uv_msg_t* socket = (uv_msg_t*)connect->handle;
	char *msg;

	free(connect);

	if (status < 0) {
		fprintf(stderr, "Connection error: %s\n", uv_strerror(status));
		return;
	}

	// we are connected! start the reading messages on this stream (asynchronously) 
	uv_msg_read_start(socket, fn_new_buffer, fn_on_msg_received, fn_delete_buffer);

	// now send some messages
	static int currentTimeSession = time_start();
	string& payloadSize = get_appconfig().get().payloadSize();

	PayloadGenerator m;

	for (int i = 0; i < get_appconfig().get().pubCount(); i++) {
		//msg = "Is it working?";
		msg = (char *)m.getPayload(payloadSize); // "Is it working?";
		send_message(socket, msg, strlen(msg) + 1, UV_MSG_STATIC, fn_on_msg_sent, (void*)126);
		//	std::this_thread::sleep_for(std::chrono::milliseconds(630));
	}
	// send a final message
	msg = "Is it working?";
	send_message(socket, msg, strlen(msg) + 1, UV_MSG_STATIC, fn_on_msg_sent, (void*)126);
};