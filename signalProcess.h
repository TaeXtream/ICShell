#pragma once

#include <deque>
#include <sys/types.h>

using namespace std;

extern bool terminalState;

void mainSignalHandler(int signal);

void pausedProcess(pid_t pid);

void resumeProcess(pid_t pid);

void killProcess(int signal);

void sigchildHandler(int signal);