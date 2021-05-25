#include "exRunner.h"
#include <cassert>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "signalProcess.h"


pid_t childID;
static int run = 1;
static int paused = 0;



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

int runExternalCommand(deque<string> commandList)
{
  assert(commandList.size() > 0);
  int exitNum = 0;
  int status;
  pid_t pid;
  if ((pid=fork()) < 0)
  {
    perror("Fork failed");
    exit(errno);
  }
  if (pid == 0)
  {
    // Reference: https://stackoverflow.com/questions/35247570/execvp-not-working-when-converting-from-vectorstring-to-vectorchar-to-char
    vector<char*> argc;
    for (auto const &a : commandList)
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

