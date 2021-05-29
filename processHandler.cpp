#include "processHandler.h"
#include "signalProcess.h"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cassert>
#include <vector>
#include <sys/wait.h>

int paused = 0;
int run = 1;

pid_t shellID;
pid_t childID;

void printProcessList(deque<ProcessNode> processQueue)
{


}

ProcessNode createProcessNode(deque<string> commandQueue)
{
    ProcessNode pnode;
    


    return pnode;
}

void sigHandler(int signal)
{
  switch (signal)
  {
  case SIGTSTP:
    if(paused){
      resumeProcess(childID);
      paused = 0;
    }
    else{
      pausedProcess(childID);
      paused = 1;
    }
    break;
  case SIGINT:
    killProcess(childID);
  default:
    break;
  }
}

int processHandler(deque<string> commandQueue)
{
    assert(commandQueue.size() > 0);
    int exitNum = 0;

    if (commandQueue[0] == "jobs")
    {
        if (commandQueue.size() > 1)
        {
            cout << "Bad Command!" << endl;
            return -1;
        }
        cout << "Print All Jobs" << endl;
        return exitNum;
    }
    pid_t pid = fork();
    int status;
    if (pid < 0)
    {
        perror("Fork failed");
        exit(errno);
        }
        if (pid == 0)
        {
            // Reference: https://stackoverflow.com/questions/35247570/execvp-not-working-when-converting-from-vectorstring-to-vectorchar-to-char
        vector<char*> argc;
        for (auto const &a : commandQueue)
            argc.emplace_back(const_cast<char*>(a.c_str()));
        argc.push_back(nullptr);
        /* This is the child, so execute the ls */ 
        exitNum = execvp(argc[0], argc.data());
    }
    else
    {
        struct sigaction exsaSTOP;
        struct sigaction exsaINT;
        exsaSTOP.sa_handler = &sigHandler;
        exsaINT.sa_handler = &sigHandler;

        childID = pid;

        while (run && waitpid(pid, &status, WNOHANG) == 0)
        { 
        sigaction(SIGINT,&exsaINT, nullptr); 
        sigaction(SIGTSTP,&exsaSTOP, nullptr);
        }
    }
    return exitNum;
}