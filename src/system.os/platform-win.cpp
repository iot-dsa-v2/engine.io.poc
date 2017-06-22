#include "platform-win.h"
#include "platform.h"

BOOL WINAPI  platform_win::ConsoleHandler(DWORD dwType)
{
	vector<string> noargs;
	switch (dwType) {
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_C_EVENT:
		printf("ctrl-c called by user --- terminating. \n");
		platform::get_instance().get()->terminate();

		break;
	case CTRL_BREAK_EVENT:
		printf("break\n");
		break;
	default:
		printf("Some other event\n");
	}

	return TRUE;
}

void platform_win::init()
{
	if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)ConsoleHandler, TRUE)) {
		fprintf(stderr, "Unable to install handler!\n");
		return;
	}
}

void platform_win::start()
{
	//DWORD dwWaitResult = WaitForSingleObject(terminateEvent, INFINITE);
}

void platform_win::terminate()
{
	//signal_process_terminal_event();
	ExitProcess(0);
}

int platform_win::time_start()
{
	int timeStampId = tsSessions++;
	TS *ts = new TS(timeStampId);
	timeStamps[timeStampId] = ts;

	return timeStampId;
}

double platform_win::time_end(int timeStampId)
{
	TS *ts = timeStamps[timeStampId];

	// stop timer
	QueryPerformanceCounter(&ts->t2);

	// compute and print the elapsed time in millisec
	ts->elapsedTime = (ts->t2.QuadPart - ts->t1.QuadPart) * 1000.0 / ts->frequency.QuadPart;

	return ts->elapsedTime;
}

