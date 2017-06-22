
//-------------------------------------------------------------------------------------------------------
// Copyright 2014 DGLogik Inc. All rights reserved.
// Licensed under the Apache License, Version 2.0 (the "License");
// See LICENSE.txt file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------
#include "platform.h"
#include <iostream>

smart_platform platform::_dsav2_instance = 0;

int main(int argc, char* argv[])
{	
	smart_platform v2 = platform::get_instance();

	if (v2.get()->init(argc, argv)) {
	
		printf("hello from iot_dsa_v2!\n");

		v2.get()->run();
	}

	return 0;
}