#include "platform-linux.h"
#include "platform.h"

#include <string.h>
#include <iostream>

using namespace std;

void platform_linux::hdl(int sig, siginfo_t *siginfo, void *context)
{
	printf("Sending PID: %ld, UID: %ld\n",
		   (long)siginfo->si_pid, (long)siginfo->si_uid);
}

void platform_linux::init()
{
	cout << "initializing the platform" << endl;

	struct sigaction act;

	memset(&act, '\0', sizeof(act));

	// Use the sa_sigaction field because the handles has two additional parameters
	act.sa_sigaction = &hdl;

	// The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler.
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGTERM, &act, NULL) < 0) {
		perror("sigaction");
		return;
	}
}

void platform_linux::start()
{

}

void platform_linux::terminate()
{
	//signal_process_terminal_event();

	exit(0);
}

int platform_linux::time_start()
{

}

double platform_linux::time_end(int timeStampId)
{

}