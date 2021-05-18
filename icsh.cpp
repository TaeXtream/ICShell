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
#include <fstream>
#include "exRunner.cpp"


using namespace std;

#define EXITCMD "exit"
#define ECHOCMD "echo"

string prevInput;
int exitNumber = 0;

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
    if (strcmp(commandQueue[0].c_str(), "$?") == 0)
    {
        cout << exitNumber << endl;
    }
    else{
        for(int i = 0; i < commandQueue.size(); i++)
        {
            cout << commandQueue[i] << " ";
        }
        cout << endl;
    }
    return;
}

int commandHandler(deque<string> commandQueue)
{
    string command = commandQueue[0];
    if (strcmp(command.c_str(), ECHOCMD) == 0) 
    {
        commandQueue.pop_front();
        doEcho(commandQueue);
        return 0;
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
        return runExternalCommand(commandQueue);;
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

void mainSignalHandler(int signal)
{
    cout << endl;
}

void mainLoop()
{
    while(true)
    {
        if (errno == EINTR) cin.clear();
        cout <<"icsh> ";
        string inputLine = waitForInput();
        if (inputLine.empty()) continue;

        if (strcmp(inputLine.c_str(), "!!") == 0) inputLine = prevInput;
        prevInput = inputLine;

        deque<string> argv = getArgumentQueue(inputLine);
        commandHandler(argv);
    }
}

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        string fileloc(argv[1]);
        processScript(fileloc);
        return exitNumber;
    }
    struct sigaction saSTOP;
    struct sigaction saINT;
    saSTOP.sa_handler = &mainSignalHandler;
    saINT.sa_handler = &mainSignalHandler;
    sigaction(SIGTSTP, &saSTOP, nullptr);
    sigaction(SIGINT, &saINT, nullptr);
    
    mainLoop();
    return exitNumber;
}
