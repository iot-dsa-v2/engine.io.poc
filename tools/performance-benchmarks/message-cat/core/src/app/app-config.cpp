//-------------------------------------------------------------------------------------------------------
// Copyright 2014 DGLogik Inc. All rights reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
// See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "stdafx.h"
#include "app-config.h"

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

unique_ptr<AppConfig> AppConfig::appConfig; // single instance

//-------------------------------------------------------------------------------------------------------
// Factory Methods
//-------------------------------------------------------------------------------------------------------
unique_ptr<AppConfig> make_appconfig(int argc, char* argv[])
{
	unique_ptr<AppConfig> appConfigInstance = make_unique<AppConfig>(argc, argv);

	if (!appConfigInstance.get()->validate()) {
		throw invalid_argument("check the args passed into application. validation failed!");
	}

	return (std::move(appConfigInstance));
}

reference_wrapper<AppConfig> get_appconfig()
{
	return *AppConfig::appConfig;
}

//-------------------------------------------------------------------------------------------------------
// Class AppConfig
//-------------------------------------------------------------------------------------------------------

AppConfig::AppConfig()
{
	cout << "AppConfig constructor" << endl;
}

AppConfig::AppConfig(int argc, char* argv[]) : nAttribs(argc), vAttribs(argv)
{
	validConfig = parseAttributes();
}

AppConfig::~AppConfig()
{
	cout << "AppConfig destructor" << endl;
}

string& AppConfig::payloadSize()
{
	return payloadRequested;
}

AppConfig* AppConfig::ipAddress(string& ipAddress)
{
	endpointIpAddress = ipAddress;
	return this;
}

string& AppConfig::ipAdress()
{
	return endpointIpAddress;
}

int AppConfig::defaultPort()
{
	return DEFAULT_SERVER_PORT; // ToDo get from app args
}

AppConfig* AppConfig::setnArgs(int argc)
{
	nAttribs = argc;

	return this;
}

AppConfig* AppConfig::setvArgs(char** argv)
{
	vAttribs = argv;

	return this;
}

AppConfig* AppConfig::opMode(SERVICE_OP_MODE mode)
{
	defaultOpMode = mode;

	return this;
}

SERVICE_OP_MODE AppConfig::opMode()
{
	return defaultOpMode;
}

AppConfig* AppConfig::pubCount(int nMessages)
{
	numberToPublish = std::to_string(nMessages);

	return this;
}

int AppConfig::pubCount()
{
	return std::stoi(numberToPublish);
}



bool AppConfig::parseAttributes()
{
	// no arguments - defaults to a pubsub service
	if (nAttribs <= 1) {
		return false;
	}

	bool findValue = false;
	string* valueReceptor = nullptr;
	for (int i = 1; i < nAttribs; ++i) {
		std::string arg = vAttribs[i];
		if ((arg == "-h") || (arg == "--help")) {
			show_usage(vAttribs[0]); 
			return true;
		}
		else if ((arg == "1b") || (arg == "1B")) {
			payloadRequested = arg;
		}
		else if ((arg == "2b") || (arg == "2B")) {
			payloadRequested = arg;
		}
		else if ((arg == "4b") || (arg == "4B")) {
			payloadRequested = arg;
		}
		else if ((arg == "8b") || (arg == "8B")) {
			payloadRequested = arg;
		}
		else if ((arg == "64b") || (arg == "64B")) {
			payloadRequested = arg;
		}
		else if ((arg == "128b") || (arg == "128B")) {
			payloadRequested = arg;
		}
		else if ((arg == "1024b") || (arg == "1024B")) {
			payloadRequested = arg;
		}
		else if ((arg == "8k") || (arg == "8K")) {
			payloadRequested = arg;
		}
		else if ((arg == "64k") || (arg == "64K")) {
			payloadRequested = arg;
		}
		else if ((arg == "128k") || (arg == "128K")) {
			payloadRequested = arg;
		}
		else if (arg == "-ip" || (arg == "endpointIp") ) {
			findValue = true;
			valueReceptor = &endpointIpAddress;
		}
		else if (arg == "-n" || (arg == "npub")) {
			findValue = true;
			valueReceptor = &numberToPublish;
		}
		else if (findValue) {
			*valueReceptor = arg;
			findValue = false;
		}
	}

	return true;
}

AppConfig* AppConfig::validated(bool value)
{
	validConfig = value;
	return this;
}

bool AppConfig::validate()
{
	return parseAttributes();
}

void AppConfig::show_usage(std::string name)
{
	std::cerr << "Usage: " << name << " <option(s)>"
		<< "Options:\n"
		<< "\t-h,--help\t\tShow this help message\n"
		<< "\t-n, count\tTotal number of messages to publish\n"
		<< "\t1b|2b|4b|8b|64b|128b|1024b|8k|64k|128k\n"
		<< "\t-ip x.x.x.x\t destination ip address"
		<< std::endl;
}
