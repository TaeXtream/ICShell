#pragma once

#include <deque>
#include <string>
#include <sys/wait.h>

using namespace std;

int runExternalCommand(deque<string> commandList);