#include "systembus.h"


systembus::systembus(std::shared_ptr<platform_config> config) : sysConfig(config)
{
	ioAdapter = make_adapter();
}


systembus::~systembus()
{
}

void systembus::start()
{
	puts("Waiting for connections ...");

	ioAdapter.get()->processIO(true);
}

void systembus::stop()
{
	ioAdapter.get()->processIO(false);
}

shared_ptr<StreamIO_UV> systembus::make_adapter()
{
    OperatingContext = make_OperatingContext();

	SERVICE_OP_MODE pubSubMode = sysConfig.get()->opMode();
	OperatingContext.setServiceIOMode(pubSubMode);
	
	return make_shared<StreamIO_UV>(OperatingContext);
}


/**
* TODO: reads system config file and sets up io context.
**/
OpContext& systembus::make_OperatingContext()
{
	SERVICE_IP = "10.42.0.103";
	OperatingContext.setServiceIP(SERVICE_IP);
	OperatingContext.setServicePort(SERVICE_LISTENER_PORT);
	OperatingContext.setServiceIOMode(OP_PUB_SUB);

	return OperatingContext;
}