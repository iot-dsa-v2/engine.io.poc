#pragma once
#include "operating-context.h"
#include "uv.h"
#include <cassert>
#include <vector>
#include <map>

using namespace std;

// metrics
typedef struct IOMetrics {
	int read_cb_called = 0;
	int write_cb_called = 0;
	int close_cb_called = 0;
} IOMetrics;



class StreamIO_UV
{	
	OpContext& operatingContext;

	// tcp server instance
	static uv_tcp_t netService;
	static uv_tcp_t netPublisher;

	// using default event loop driver
	static uv_loop_t *loop;

	// peer IOEndPoint address
	struct sockaddr_in ioEndPoint;
	static const int READ_BUFFER_MAX = 65536;
	static char ring[READ_BUFFER_MAX];

	// metrics
	static const int pubCount = 1000;
	static const int pubInterval = 15;
	static IOMetrics ioMetrics;
public:
	StreamIO_UV(OpContext& OperatingContext);
	virtual ~StreamIO_UV();

	void processIO(bool run);
private:
	int enable();
	int disable();

	void stopListener();
	void startListener(uint16_t servicePort);
	void provisionListener();
	void provisionPublisher();
	void stopPublisher();

	void makeServiceEndPoint(OpContext& opContext);

	// UV callback APIs 
	static void connection_cb(uv_stream_t * server, int status);
	static void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	static void read_cb(uv_stream_t * stream, ssize_t nread, const uv_buf_t* buf);
	static void on_publisher_close(uv_handle_t* handle);
	static void on_publish(uv_write_t* req, int status);
	static void on_start_publish(uv_connect_t* connection, int status);
	static void doWrite(uv_stream_t* stream);
	static void on_walk_uvactives(uv_handle_t* handle, void* arg);
	static void on_uv_close(uv_handle_t* handle);
private:
	static vector<int> timeStamps;
};