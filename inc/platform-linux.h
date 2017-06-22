#pragma once
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <uv.h>

class platform_linux
{
public:
    platform_linux() {};
    void init();
    void start();
    void terminate();
    int time_start();
    double time_end(int timeStampId);

private:
    static void hdl(int sig, siginfo_t *siginfo, void *context);
    void make_process_terminal_event();
    void signal_process_terminal_event();

private:
};
