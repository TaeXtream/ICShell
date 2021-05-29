#pragma once

#include <deque>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;


struct ProcessNode{
    pid_t data;
    string state;
    string command;
};

static deque<ProcessNode> processList;

int processHandler(deque<string> commandQueue);