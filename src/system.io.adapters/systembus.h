#pragma once
#include "operating-context.h"
#include "stream.io-uv.h"
#include "platform-config.h"
#include <memory>

/**
  * Own the socket connections from peers.
  **/
class systembus
{
	// this data will eventually come from a config
	string SERVICE_IP;
	static const uint16_t SERVICE_LISTENER_PORT = 8888;
public:
	systembus(std::shared_ptr<platform_config> config);
	virtual ~systembus();

	void start();
	void stop();

private:
    std::shared_ptr<StreamIO_UV> make_adapter();
	OpContext& make_OperatingContext();


private:
	OpContext OperatingContext;
    std::shared_ptr<StreamIO_UV> ioAdapter;
	std::shared_ptr<platform_config> sysConfig;
};

