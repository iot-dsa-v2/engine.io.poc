#include "stdafx.h"
#include "CppUnitTest.h"

// target include files
#include "app-config.h"
#include "tcp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace messagecatTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Test_AppConfig_commandline_print_help)
		{
			// Test Setup 
			AppConfig::appConfig = nullptr;
			int		argc = 2;
			char *	argv[] = { "messagecat-tests", "-h" };

			try {
				AppConfig::appConfig = make_appconfig(argc, argv);
			}
			catch (...) {
				// ...
				exit(1);
			}			
		}

	};
}