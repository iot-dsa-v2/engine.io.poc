#include "platform-config.h"

#include <iostream>
#include <vector>
using namespace std;

//platform_config::platform_config(int argc, char** argv) : nAttribs(argc), vAttribs(argv)
//{
//}

platform_config::platform_config(int argc, char* argv[]) : nAttribs(argc), vAttribs(argv)
{
	validConfig = parseAttributes();
}

platform_config::~platform_config()
{
}

platform_config* platform_config::setnArgs(int argc)
{
	nAttribs = argc;

	return this;
}

platform_config* platform_config::setvArgs(char** argv)
{
	vAttribs = argv;

	return this;
}

platform_config* platform_config::opMode(SERVICE_OP_MODE mode)
{
	defaultOpMode = mode;

	return this;
}

SERVICE_OP_MODE platform_config::opMode()
{
	return defaultOpMode;
}


bool platform_config::parseAttributes()
{
	// no arguments - defaults to a pubsub service
	if (nAttribs <= 1) {
		opMode(OP_PUB_SUB);

		return true;
	}

	for (int i = 1; i < nAttribs; ++i) {
		std::string arg = vAttribs[i];
		if ((arg == "-h") || (arg == "--help")) {
			show_usage(vAttribs[0]);
			return false;
		}
		else if ((arg == "-p") || (arg == "--pub")) {
			opMode(OP_PUB);
		}
		else if ((arg == "-s") || (arg == "--sub")) {
			opMode(OP_SUB);
		}
		else if ((arg == "-ps") || (arg == "--pubsub")) {
			opMode(OP_PUB);
		}
	}

	return true;
}

platform_config* platform_config::validated(bool value)
{
	validConfig = value;
	return this;
}

bool platform_config::valid()
{
	return validConfig;
}

void platform_config::show_usage(std::string name)
{
	std::cerr << "Usage: " << name << " <option(s)>"
		<< "Options:\n"
		<< "\t-h,--help\t\tShow this help message\n"
		<< "\t-p,--pub PUB\tStart Publisher"
		<< "\t-s,--sub SUB\tStart Subscriber"
		<< std::endl;
}