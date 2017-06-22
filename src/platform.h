#pragma once
#include "platform-universal.h"
#include "platform-config.h"
#include "systembus.h"

#include <stdio.h>
#include <memory>

class platform;

typedef std::shared_ptr<platform> smart_platform;

class platform
{
public:
	static smart_platform get_instance();
	static smart_platform make_dsav2();
	void run();
	void terminate();
	bool init(int argc, char* argv[]);

	int time_start();
	double time_end(int timeStampId);
protected:
	platform();
	//virtual ~platform();

private:
	shared_ptr<systembus> make_systembus();
	shared_ptr<platform_config> make_platform_config(int argc, char* argv[]);
private:
	static smart_platform _dsav2_instance;
	shared_ptr<systembus> sysBus;
	platform_linux osPlatform;
	shared_ptr<platform_config> config;
};
