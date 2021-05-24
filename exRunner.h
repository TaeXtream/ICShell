#pragma once

#include <deque>
#include <string>
#include <sys/wait.h>

using namespace std;

void suspend_process(pid_t pid);
void resume_process(pid_t pid);
void sigINThandler(int signal);
void sigSTOPhandler(int signal);

int runExternalCommand(deque<string> commandList);