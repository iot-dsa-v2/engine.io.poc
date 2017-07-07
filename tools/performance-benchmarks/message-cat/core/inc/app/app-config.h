#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

typedef enum {
	OP_PUB,
	OP_SUB,
	OP_PUB_SUB
} SERVICE_OP_MODE;

class AppConfig
{
private:
	int		nAttribs;
	char**	vAttribs;
	string	payloadRequested;
	string	numberToPublish;

	// public Interface
public:
	AppConfig();
	AppConfig(int argc, char* argv[]);
	virtual ~AppConfig();

	AppConfig* ipAddress(string& ipAddress);
	string& ipAdress();

	int defaultPort();

	SERVICE_OP_MODE opMode();
	AppConfig* opMode(SERVICE_OP_MODE mode);
	string& payloadSize();

	AppConfig* validated(bool value);
	bool validate();

	AppConfig* pubCount(int nMessages);
	int pubCount();


	AppConfig* setnArgs(int argc);
	AppConfig* setvArgs(char** argv);

	bool parseAttributes();
	void show_usage(string name);

private:
	//vector<string> userArgs;
	SERVICE_OP_MODE defaultOpMode;
	bool validConfig;
	string endpointIpAddress;
public:
	static unique_ptr<AppConfig> appConfig;
	static const int DEFAULT_SERVER_PORT = 8888;
};

// factory Methods
unique_ptr<AppConfig> make_appconfig(int argc, char* argv[]);
reference_wrapper<AppConfig> get_appconfig();