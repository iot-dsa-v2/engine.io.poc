#pragma once
#include "platform-config.h"

#include <stdint.h>
#include <string>
using namespace std;

/**
  * Defines Service end point settings and other transport
  * related configurations.
  **/
class OpContext
{
	string serviceIP;
	uint16_t servicePort;
public:
	OpContext();
	~OpContext();

	OpContext* setServiceIP(string& ipAddr);
	OpContext* setServicePort(uint16_t port);
	OpContext* setServiceIOMode(SERVICE_OP_MODE ioMode);
	uint16_t getServicePort();
	string& getServiceIP();
	SERVICE_OP_MODE getServiceIOMode();
private:
	SERVICE_OP_MODE pubsubMode;
};

