#pragma once

#include <sys/types.h>

using namespace std;

void mainSignalHandler(int signal);

void pausedProcess(pid_t pid);

void resumeProcess(pid_t pid);

void killProcess(int signal);