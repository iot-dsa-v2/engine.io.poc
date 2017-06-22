#pragma once
#define WIN32_LEAN_AND_MEAN   
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <uv.h>
#include "systembus.h"

class platform_win
{
	typedef struct TS {
		int tsId;
		LARGE_INTEGER frequency;        // ticks per second
		LARGE_INTEGER t1, t2;           // ticks
		double elapsedTime;

		TS(int id) : tsId(id) {
			// get ticks per second
			QueryPerformanceFrequency(&frequency);

			// start timer
			QueryPerformanceCounter(&t1);
		}

	} TS;

public:
	platform_win() {};
	void init();
	void start();
	void terminate();
	int time_start();
	double time_end(int timeStampId);

private:
	static BOOL WINAPI ConsoleHandler(DWORD dwType);
	void make_process_terminal_event();
	void signal_process_terminal_event();

private:
	HANDLE terminateEvent;
	int tsSessions;
	TS* timeStamps[10];
};
