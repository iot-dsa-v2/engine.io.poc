#include "platform.h"

#include <string.h>
#include <iostream>
#include <memory>

using namespace std;

smart_platform platform::get_instance()
{
	if (_dsav2_instance == nullptr) {
		_dsav2_instance = make_dsav2();
	}

	return _dsav2_instance;
}

smart_platform platform::make_dsav2()
{
	smart_platform dsaBroker(new platform());

	return dsaBroker;
}

platform::platform()
{
	//init();
}


//platform::~platform()
//{
//}

bool platform::init(int argc, char* argv[])
{
	bool ret = true;

	config = make_platform_config(argc, argv);
	if (!config.get()->valid()) {
		return false;
	}

	cout << "initializing the platform" << endl;

	sysBus = make_systembus();
	osPlatform.init();

	return ret;
}

void platform::run()
{
	sysBus.get()->start();
}

void platform::terminate()
{
    sysBus.get()->stop();
	
	osPlatform.terminate();
}

shared_ptr<systembus> platform::make_systembus()
{
	shared_ptr<systembus> smartp = make_shared<systembus>(config);

	return move(smartp);
}

shared_ptr<platform_config> platform::make_platform_config(int argc, char* argv[])
{
	shared_ptr<platform_config> smartp = make_shared<platform_config>(argc, argv);

	return move(smartp);
}

int platform::time_start()
{
	return osPlatform.time_start();
}

double platform::time_end(int timeStampId)
{
	return osPlatform.time_end(timeStampId);
}
