#pragma once
#include <string>
#include <vector>

using namespace std;

typedef enum {
	OP_PUB,
	OP_SUB,
	OP_PUB_SUB
} SERVICE_OP_MODE;

class platform_config
{
private:
	int		nAttribs;
	char**	vAttribs;
public:
	platform_config(int argc, char* argv[]);
	virtual ~platform_config();

	SERVICE_OP_MODE opMode();
	platform_config* opMode(SERVICE_OP_MODE mode);
	platform_config* validated(bool value);
	bool valid();

	platform_config* setnArgs(int argc);
	platform_config* setvArgs(char** argv);

	bool parseAttributes();
	void show_usage(string name);
private:
	//vector<string> userArgs;
	SERVICE_OP_MODE defaultOpMode;
	bool validConfig;
};

