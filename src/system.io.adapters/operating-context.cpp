#include "operating-context.h"

OpContext::OpContext()
{
}

OpContext::~OpContext()
{
}

uint16_t OpContext::getServicePort()
{
	return servicePort;
}

OpContext* OpContext::setServiceIP(string& ipAddr)
{
	serviceIP = ipAddr;

	return this;
}

string& OpContext::getServiceIP()
{
	return serviceIP;
}

OpContext* OpContext::setServicePort(uint16_t port)
{
	servicePort = port;

	return this;
}

OpContext* OpContext::setServiceIOMode(SERVICE_OP_MODE opMode)
{
	pubsubMode = opMode;

	return this;
}

SERVICE_OP_MODE OpContext::getServiceIOMode()
{
	return pubsubMode;
}

