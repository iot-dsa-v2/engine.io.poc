#include "stream.io-uv.h"
#include "platform.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string.h>

using namespace std;

uv_tcp_t StreamIO_UV::netService;
uv_tcp_t StreamIO_UV::netPublisher;
uv_loop_t *StreamIO_UV::loop = nullptr;
vector<int> StreamIO_UV::timeStamps;
const int StreamIO_UV::pubInterval;

struct MessagePub {
	char size = 128;
	string data = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567";
	
	string payload_1K = data + data + data + data + data + data + data + data + data + data;
	string payload_64K = payload_1K + payload_1K + payload_1K + payload_1K + payload_1K + payload_1K + payload_1K + payload_1K + payload_1K + payload_1K;

	MessagePub() {
	}

	MessagePub(char* buf) {
		buf[129] = '\0';
		cout << "Received" << buf << endl;
	}

	const char* getPayload() {
		return payload_1K.c_str();
	}
} m;

struct MessageSub {
	stringstream ringBuffer;
	int nPackets = 0;
	MessageSub() {

	}

	void parse(char *data, int len) {
		//if (len > 128) {
		//	data[128] = '\0';
		//}
		//ringBuffer << data;
		//processBuffer(in);
		//cout << ".";
		//if (nPackets++ % 50 == 0) {
			//cout << data << endl;
			//cout << len << endl;
			//cout << endl << nPackets << endl;
		//}
		//else {
		if (len == 0) {
			return;
		}
			//processBuffer(data, len);
		//}
		//print(data, len);

		//int r = signIt(data, len);
		//r = signIt(data, len);
		cout << nPackets++ << endl;
	}

	int signIt(char *data, int len) {
		int r = 0;
		for (int i = 0; i < 64 * 5; i++) {
			r %= data[i] + i;
		}

		return r;
	}

	void processBuffer(char *data, int len) {
		char workBuffer[129];
		workBuffer[128] = '\0'; 
		int n = 0;
		int count = 0;
		while (true) {

			if ((n+128) >= len ) {
				break;
			}
			memcpy(workBuffer, &data[n], 128);
			int sig = 0; // signIt(workBuffer, 128);
			count++;
			cout << "#" << count << "{" << sig << "}" << endl;

			n += 128;
		}
	}

	void print(char *data, int len) {
		if (len < 128) {
			cout << data << endl;
			return;
		}

		int curr_v = 0;
		char *myC = data;
		while (true) {
			if (curr_v >= len) {
				break;
			}

			printf("%c", myC[curr_v]);

			curr_v++;
			if ((curr_v % 128) == 0) {
				cout << endl;
			}
		}
		cout << "==================" << endl;
	}
} s;


StreamIO_UV::StreamIO_UV(OpContext& OperatingContext) :
	operatingContext(OperatingContext)
{
}

StreamIO_UV::~StreamIO_UV()
{
}

void StreamIO_UV::processIO(bool run)
{
	(run) ? enable(): disable();
}

int StreamIO_UV::enable()
{
	switch (operatingContext.getServiceIOMode())
	{
		case OP_PUB_SUB: {
			provisionListener();
			startListener(operatingContext.getServicePort());
			provisionPublisher();
		}
		case OP_PUB: {
			provisionPublisher();
		}
		case OP_SUB: {
			provisionListener();
			startListener(operatingContext.getServicePort());
		}
	}

	return 0;
}

int StreamIO_UV::disable()
{
	switch (operatingContext.getServiceIOMode())
	{
	case OP_PUB_SUB: {
			stopListener();
			stopPublisher();
		}
	case OP_PUB: {
		stopPublisher();
		}
	case OP_SUB: {
		stopListener();
		}
	}

	return 0;
}

void StreamIO_UV::startListener(uint16_t servicePort)
{
	/* let the netIOService listen on the address for new connections */
	int r = uv_listen((uv_stream_t *)&netService, 128, connection_cb);
	if (r) {
		return;
		fprintf(stderr, "Error on listening:.\n");
	}

	/* execute all tasks in queue */
	uv_run(loop, UV_RUN_DEFAULT);
}

void StreamIO_UV::stopListener()
{
	uv_stop(loop);

	int activeHandles = 0;
	uv_walk(loop, on_walk_uvactives, (void*)&activeHandles);

	/* gives all close callbacks a chance to run */
	uv_run(loop, UV_RUN_DEFAULT);
	uv_loop_close(loop);
}

void StreamIO_UV::stopPublisher()
{
	uv_stop(loop);

	int activeHandles = 0;
	uv_walk(loop, on_walk_uvactives, (void*)&activeHandles);

	/* gives all close callbacks a chance to run */
	uv_run(loop, UV_RUN_DEFAULT);
	uv_loop_close(loop);
}

void StreamIO_UV::provisionListener()
{
	loop = uv_default_loop();
	
	/* initialize the netIOService */
	uv_tcp_init(loop, &netService);

	/* defines an endpoint for the Service to listen on - for connections */
	makeServiceEndPoint(operatingContext);

	/* bind the netIOService to the address above */
	unsigned int flags = 0;
	uv_tcp_bind(&netService, (struct sockaddr *) &ioEndPoint, flags);
}

void StreamIO_UV::provisionPublisher()
{
	loop = uv_default_loop();

	/*const uv_buf_t buffer[] = {
		{ 128 + 1, (char *)m.getPayload() },
		{ 128 + 1, (char *)m.getPayload() }
	};

	loop->data = (uv_buf_t *)buffer;
*/

	/* initialize the netIOService */
	uv_tcp_init(loop, &netPublisher);

	// creates a TCP end point address / ip:port address when adapter is in listening mode.
	uv_ip4_addr(operatingContext.getServiceIP().c_str(), operatingContext.getServicePort(), &ioEndPoint);

	uv_tcp_keepalive(&netService, 1, 60);


	uv_connect_t connect;
	uv_tcp_connect(&connect, &netPublisher, (struct sockaddr *) &ioEndPoint, on_start_publish);

	uv_run(loop, UV_RUN_DEFAULT);
}

void StreamIO_UV::makeServiceEndPoint(OpContext& opContext)
{
	// creates a TCP end point address / ip:port address when adapter is in listening mode.
	uv_ip4_addr(operatingContext.getServiceIP().c_str(), operatingContext.getServicePort(), &ioEndPoint);
}

/**
||||||||||||||||||||||||||||||||||||||||||||
	Async IO here : Managed UV Callbacks
*/

char StreamIO_UV::ring[READ_BUFFER_MAX];
IOMetrics StreamIO_UV::ioMetrics;

/**
* Callback executes on each new connection.
**/
void StreamIO_UV::connection_cb(uv_stream_t * server, int status)
{
	static int count = 0;
	/* dynamically allocate a new client stream object on conn */
	uv_tcp_t * client = new uv_tcp_t;

	/* if status not zero there was an error */
	if (status == -1) {
		fprintf(stderr, "Error on listening:\n");
	}
	cout << "client #" << ++count << endl << "connects";

	/* initialize the new client */
	uv_tcp_init(loop, client);

	/* now let bind the client to the server to be used for incomings */
	if (uv_accept(server, (uv_stream_t *)client) == 0) {

		/* start reading from stream */
		/*smart_platform v2 = platform::get_instance();
		int tsId = v2.get()->time_start();
		timeStamps.push_back(tsId);*/
		int r = uv_read_start((uv_stream_t *)client, (uv_alloc_cb)alloc_buffer, read_cb);

		if (r) {
			fprintf(stderr, "Error on reading client stream\n");
			///fprintf(stderr, "Error on reading client stream: %s.\n",
			//uv_strerror(uv_last_error(loop)));
		}

		cout << "Accepted Peer Connection Request  " << count++ << endl;
	}
	else {
		/* close client stream on error */
		uv_close((uv_handle_t *)client, NULL);
	}
}


/**
* Callback which is executed on each readable state.
*/
void StreamIO_UV::read_cb(uv_stream_t * stream, ssize_t nread, const uv_buf_t* buf)
{
	/* if read bytes counter -1 there is an error or EOF */
	if (nread == -1) {
		cout << "*****Error on reading client stream: " << endl;
		uv_close((uv_handle_t *)stream, NULL);
		return;
	}

	if (nread > 0 && buf->len > 0) {
		int availableLength = buf->len;
		if (nread < buf->len) {
			availableLength = nread;
		}
		uv_buf_t receivedBuffer;
		receivedBuffer.base = buf->base;
		receivedBuffer.len = availableLength;
		s.parse(receivedBuffer.base, availableLength);
		//std::this_thread::sleep_for(std::chrono::milliseconds(50));

		/* dynamically allocate memory for a new write task */
		/**
		uv_write_t * req = (uv_write_t *)malloc(sizeof(uv_write_t));

		// write sync the incoming buffer to the socket
		int r = uv_write(req, stream, (const uv_buf_t*) &receivedBuffer, 1, NULL);
		if (r) {
			fprintf(stderr, "Error on writing client stream:.\n");
		}
		*/
	}
	else if (nread == 0) {
		cout << "nread is 0 EAGAIN reached" << endl;
	}
	else if (nread == UV_EOF) {
		cout << "EOF reached for read stream" << endl;
		/*smart_platform v2 = platform::get_instance();
		for (int tsId : timeStamps) {
			cout << "Elapsed time for TSId " << tsId << "= " << v2.get()->time_end(tsId);
		}
		//std::cout << "elapsed time = " << v2.get()->time_end() << endl;*/
	}

	/* free the remaining memory */
	//free(buf->base);
}

/**
* Allocates a buffer which we can use for reading.
**/
void StreamIO_UV::alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	if (suggested_size > READ_BUFFER_MAX) {
		//cout << "Read buffer size " << suggested_size << " exceeds Max Size. Resetting to 1K" << endl;
		suggested_size = READ_BUFFER_MAX;
	}

	//return uv_buf_init(new char[suggested_size], suggested_size);
	buf->base = ring;//new char[suggested_size];
					 //buf->base = new char[suggested_size];
	buf->len = suggested_size;
}

void StreamIO_UV::on_start_publish(uv_connect_t* connection, int status)
{
	printf("connected.\n");

	uv_stream_t* stream = connection->handle;
	doWrite(stream);
}

void StreamIO_UV::on_publish(uv_write_t* req, int status)
{
	if (status) {
		cout << "uv_write error:" << endl;

		return;
	}
	ioMetrics.write_cb_called++;
	printf("write_cb %d  \n", ioMetrics.write_cb_called);
	if (ioMetrics.write_cb_called >= pubCount) {
		printf("write_cb_called equals pubCount of %d \n", pubCount);
		uv_close((uv_handle_t*)&netPublisher, on_publisher_close);

		return;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(pubInterval));
	doWrite(req->handle);
}

void StreamIO_UV::doWrite(uv_stream_t* stream) 
{
	int r;
	int nWrites = pubCount;

	if (nWrites <= 0) {
		return;
	}
	else {
		nWrites--;
	}
	
	uv_write_t* req;
	uv_buf_t buf;
	//buf = uv_buf_init("PING", 4);
	buf = uv_buf_init((char*) m.data.c_str(), 128);
	
	req = (uv_write_t* ) malloc(sizeof *req);
	assert(req != NULL);

	//r = uv_write(req, (uv_stream_t*)&netPublisher, &buf, 1, on_publish);
	r = uv_write(req, (uv_stream_t*)&netPublisher, &buf, 1, on_publish);
	assert(r == 0);
}

void  StreamIO_UV::on_publisher_close(uv_handle_t* handle) 
{
	assert(handle == (uv_handle_t*)&netPublisher);
	ioMetrics.close_cb_called++;
}

void  StreamIO_UV::on_uv_close(uv_handle_t* handle)
{
	ioMetrics.close_cb_called++;
}

void StreamIO_UV::on_walk_uvactives(uv_handle_t* handle, void* arg)
{
	int* activeHandles = (int *)arg;
	(*activeHandles)++;
}