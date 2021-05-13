// ICShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iterator>
#include <sstream>
#include <cerrno>
#include <deque>
#include <signal.h>
#include <fstream>
#include "exRunner.cpp"


using namespace std;

#define EXITCMD "exit"
#define ECHOCMD "echo"

string prevInput;

string waitForInput()
{
    string inputLine;
    getline(cin, inputLine);
    int len = inputLine.length();
    if (len == -1)
    {
        printf("ERROR: %s\n", strerror(errno));
        exit(EXIT_SUCCESS);
    }
    return inputLine;
}

deque<string> getArgumentQueue(string inputLine)
{
    stringstream stream(inputLine);
    deque<string> argv((istream_iterator<string>(stream)), istream_iterator<string>());
    return argv;
}

void doEcho(deque<string> commandQueue)
{
    for(int i = 0; i < commandQueue.size(); i++)
    {
        cout << commandQueue[i] << " ";
    }
    cout << endl;
    return;
}

void commandHandler(deque<string> commandQueue)
{
    string command = commandQueue[0];
    if (strcmp(command.c_str(), ECHOCMD) == 0) 
    {
        commandQueue.pop_front();
        doEcho(commandQueue);
        return;
    }
    else if (strcmp(command.c_str(), EXITCMD) == 0)
    {
        if (commandQueue.size() > 1)
        {
            int exitCode = stoi(commandQueue[1]);
            if (exitCode > 255)
            {
                exitCode = exitCode >> 8;
            }
            cout << "Bye" << endl;
            exit(exitCode);
        }
        cout << "Bye" << endl;
        exit(0);
    }
    else
    {
        runExternalCommand(commandQueue);
        return;
    }
}

void processScript(string scriptLoc)
{
    ifstream infile(scriptLoc);
    string line;
    while (getline(infile, line))
    {
        if (line.compare("!!") == 1) line = prevInput;
        prevInput = line;

        deque<string> argv = getArgumentQueue(line);
        commandHandler(argv);
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        string fileloc(argv[1]);
        processScript(fileloc);
    }
    else {
        string inputLine;
        while(true)
        {
            cout << "icsh> ";
            inputLine = waitForInput();
            if (inputLine.empty()) continue;

            if (strcmp(inputLine.c_str(), "!!") == 0) inputLine = prevInput;
            prevInput = inputLine;

            deque<string> argv = getArgumentQueue(inputLine);
            commandHandler(argv);
        }
    }
    return 0;
}
