// MessageCat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "app-config.h"
#include "ioprocessor.h"

#include <chrono>
#include <thread>
#include <memory>

using namespace std;

bool getUserSpecs(int argc, char* argv[]);

int main(int argc, char* argv[]) 
{
	if (!getUserSpecs(argc, argv)) {
		exit(1);
	}

	IOProcessor ioProcessor;

	ioProcessor.createDataFlowClient();
	ioProcessor.run();

	while (1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

bool getUserSpecs(int argc, char* argv[]) {
	bool valid = true;

	try {
		AppConfig::appConfig = make_appconfig(argc, argv);
	}
	catch (const std::invalid_argument& e) {
		// log it
		std::cerr << "Invalid program parameters specified: " << e.what() << '\n';
		valid = false;
	}

	return valid;
}
