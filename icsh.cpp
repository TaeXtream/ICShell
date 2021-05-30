// ICShell.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iterator>
#include <sstream>
#include <cerrno>
#include <deque>
#include <fstream>
#include <algorithm>
#include <fcntl.h>
#include "processHandler.h"


using namespace std;

#define EXITCMD "exit"
#define ECHOCMD "echo"
#define GAMECMD "game"

//ANSI Color codes
#define RED   		"\033[0;31m"
#define YELLOW 		"\033[0;33m"
#define GREEN 		"\033[0;32m"
#define CYAN 		"\033[0;36m"
#define BLUE 		"\033[0;34m"
#define MAGENTA     "\033[0;35m"
#define RESET  		"\e[0m" 

string prevInput;
int exitNumber = 0;

string waitForInput()
{
    string inputLine;
    getline(cin, inputLine);
    int len = inputLine.length();
    if (len == -1)
    {
        printf(RED "ERROR: %s\n" RESET, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return inputLine;
}

deque<string> getArgumentQueue(string inputLine)
{
    stringstream stream(inputLine);
    deque<string> argv((istream_iterator<string>(stream)), istream_iterator<string>());
    return argv;
}

int doEcho(deque<string> commandQueue)
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
    return 0;
}

int commandHandler(deque<string> commandQueue)
{
    string command = commandQueue[0];
    if (strcmp(command.c_str(), ECHOCMD) == 0) 
    {
        commandQueue.pop_front();
        exitNumber = doEcho(commandQueue);
        return exitNumber;
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
    else if (strcmp(command.c_str(), GAMECMD) == 0)
    {
        if (commandQueue.size() != 2)
        {
            cout << RED << "Bad Command!" << RESET << endl;
            return exitNumber;
        }
        string gameName = commandQueue[1];
        if (gameName == "Racing")
        {
            commandQueue.clear();
            commandQueue.push_back("./Games/Racing.exe");
        }
        else if (gameName == "FPS")
        {
            commandQueue.clear();
            commandQueue.push_back("./Games/FPS.exe");
        }
        else if (gameName == "Asteroids")
        {
            commandQueue.clear();
            commandQueue.push_back("./Games/Asteroids.exe");
        }
        else if (gameName == "help")
        {
            cout << GREEN <<"Existing Game: Asteroids, FPS, Racing" << endl;
            cout << YELLOW << "Command Format: " << CYAN << "game <Game Name>" << RESET << endl;
            return 0;
        }
        return processHandler(commandQueue);
    }
    else
    {
        return processHandler(commandQueue);
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
        exitNumber = commandHandler(argv);
    }
}


// References: http://www.cplusplus.com/forum/general/94879/
void outputRedirection(deque<string> commandQueue)
{
    auto it = find(commandQueue.begin(), commandQueue.end(), ">");
    int idx = distance(commandQueue.begin(), it);
    string fileName = commandQueue[idx+1];
    commandQueue.erase(commandQueue.begin() + idx + 1);
    commandQueue.erase(commandQueue.begin() + idx);
    int fd = open(fileName.c_str(), O_WRONLY | O_CREAT);
    if (fd < 0)
    {
        cout << RED << "Cannot Open or Create " << YELLOW << fileName << RESET << endl;
        return;
    }
    int defout = dup(1);
    dup2(fd, 1);
    exitNumber = commandHandler(commandQueue);
    dup2(defout, 1);
    close(fd);
    close(defout);
    return;
}

void inputRedirection(deque<string> commandQueue)
{
    string fileName = commandQueue.back();
    ifstream infile(fileName);
    string line;
    while (getline(infile, line))
    {
        if (line.compare("!!") == 1) line = prevInput;
        prevInput = line;

        deque<string> argv = getArgumentQueue(line);
        exitNumber = commandHandler(argv);
    }
    return;

}

void inAndOutRedirection(deque<string> commandQueue)
{
    auto it = find(commandQueue.begin(), commandQueue.end(), ">");
    int outidx = distance(commandQueue.begin(), it);
    string outFileName = commandQueue[outidx+1];

    it = find(commandQueue.begin(), commandQueue.end(), "<");
    int inidx = distance(commandQueue.begin(), it);
    string inFileName = commandQueue[inidx+1];

    int fd = open(outFileName.c_str(), O_WRONLY | O_CREAT);
    if (fd < 0)
    {
        cout << RED << "Cannot Open or Create " << YELLOW << outFileName << RESET << endl;
        return;
    }
    int defout = dup(1);
    dup2(fd, 1);
    
    ifstream infile(inFileName);
    string line;
    while (getline(infile, line))
    {
        if (line.compare("!!") == 1) line = prevInput;
        prevInput = line;

        deque<string> argv = getArgumentQueue(line);
        exitNumber = commandHandler(argv);
    }

    dup2(defout, 1);
    close(fd);
    close(defout);
    return;

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
        cout << CYAN <<"icsh> " << RESET;
        string inputLine = waitForInput();
        if (inputLine.empty()) continue;

        if (strcmp(inputLine.c_str(), "!!") == 0) inputLine = prevInput;
        prevInput = inputLine;

        deque<string> argv = getArgumentQueue(inputLine);
        bool reOut = find(argv.begin(), argv.end(), ">") != argv.end();
        bool reIn = find(argv.begin(), argv.end(), "<") != argv.end();
        if (reIn && reOut)
        {
            inAndOutRedirection(argv);
            continue;
        }
        else if (reOut && !reIn)
        {
            outputRedirection(argv);
            continue;
        }
        else if (reIn && !reOut)
        {
            inputRedirection(argv);
            continue;
        }
        else
        {
            commandHandler(argv);
        }
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
    cout << RED << "  _____ _____    _____ _    _      _ _ " << endl;
    cout << YELLOW << " |_   _/ ____|  / ____| |  | |    | | |" << endl;
    cout << GREEN << "   | || |      | (___ | |__| | ___| | |" << endl;
    cout << CYAN << "   | || |       \\___ \\|  __  |/ _ \\ | |" << endl;
    cout << BLUE << "  _| || |____   ____) | |  | |  __/ | |" << endl;
    cout << MAGENTA << " |_____\\_____| |_____/|_|  |_|\\___|_|_|" << RESET << endl;
    struct sigaction saSTOP;
    struct sigaction saINT;
    saSTOP.sa_handler = &mainSignalHandler;
    saINT.sa_handler = &mainSignalHandler;
    sigaction(SIGTSTP, &saSTOP, nullptr);
    sigaction(SIGINT, &saINT, nullptr);

    shellID = getpid();
    setpgid(shellID, shellID);
    tcsetpgrp(0, shellID);
    
    mainLoop();
    return exitNumber;
}
