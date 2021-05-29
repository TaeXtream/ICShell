#pragma once

#include <deque>
#include <string>
#include <sys/wait.h>

using namespace std;


struct ProcessNode
{
    pid_t pid;
    string state;
    string command;
};

extern deque<ProcessNode> processList;
extern pid_t shellID;
extern pid_t childID;

int processHandler(deque<string> commandQueue);
void deleteProcessfromList(pid_t id);