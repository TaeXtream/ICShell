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
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <iterator>


int paused = 0;
int run = 1;
deque<ProcessNode> processList;
pid_t shellID;
pid_t childID;



void printProcessList(deque<ProcessNode> processQueue)
{
  for (int i = 0; i < processQueue.size(); i++)
  {
    ProcessNode currentNode = processQueue[i];
    cout << "[" << i << "] " << currentNode.pid << " " << currentNode.state << " " << currentNode.command << endl;
  }
}

void deleteProcessfromList(pid_t id)
{
  for(auto it = processList.begin(); it != processList.end();++it)
  {
    if (it->pid == id)
    {
      it = processList.erase(it);
      return;
    }
  }
  
}

void updateProcessStatusInList(pid_t id, string status)
{
  for(auto it = processList.begin(); it != processList.end();++it)
  {
    if (it->pid == id)
    {
      it->state = status;
      return;
    }
  }
}

int searchProcessinList(pid_t pid)
{
  for (int i = 0; i < processList.size(); i++)
  {
    if (processList[i].pid == pid)
    {
      return i;
    }
  }
  return -1;
}

void sigHandler(int signal)
{
  switch (signal)
  {
  case SIGTSTP:
    if(paused)
    {
      resumeProcess(childID);
      updateProcessStatusInList(childID, "Running");
      paused = 0;
    }
    else
    {
      pausedProcess(childID);
      updateProcessStatusInList(childID, "Paused");
      paused = 1;
    }
    break;
  case SIGINT:
    killProcess(childID);
    deleteProcessfromList(childID);
  default:
    break;
  }
}

void bgProcessHandler()
{
    signal(SIGTTOU, SIG_IGN);
    struct sigaction bgProcessSigHandler;
    sigemptyset(&bgProcessSigHandler.sa_mask);
    bgProcessSigHandler.sa_handler = sigChildHandler;
    bgProcessSigHandler.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD,&bgProcessSigHandler, NULL) == -1)
        exit(1);
}

void parentProcessHandler(pid_t pid)
{
    int status;
    struct sigaction exsaSTOP;
    struct sigaction exsaINT;
    exsaSTOP.sa_handler = &sigHandler;
    exsaINT.sa_handler = &sigHandler;

    childID = pid;
    while (!terminalState && waitpid(pid, &status, WNOHANG) == 0)
    { 
      sigaction(SIGINT,&exsaINT, nullptr); 
      sigaction(SIGTSTP,&exsaSTOP, nullptr);
    }
    if (!terminalState)
    { 
      deleteProcessfromList(childID);
      childID = 0;
    }
}

int movingProcessFGandBG(pid_t pid, bool toFG)
{
  int processidx = searchProcessinList(pid);
  if(processidx < 0)
  {
    cout << "no process with id " << pid << endl;
    return 0;
  }
  if (toFG)
  {
    terminalState = false;
    cout << processList[processidx].command << endl;
    parentProcessHandler(pid);
    terminalState = true;
  }
  else
  {
    if (!childID)
    {
      cout << "No Child Process Running" << endl;
      return 0;
    }
    if (pid != childID)
    {
      cout << "The Process is currently running in the background" << endl;
    }
    if (terminalState)
    {
      setpgid(childID,childID);
      kill(childID, SIGCONT);
      childID = 0;
      bgProcessHandler();
    }
  }
  return 0;

}

int processHandler(deque<string> commandQueue)
{
    assert(commandQueue.size() > 0);
    int exitNum = 0;
    bool toBackground = false;
    if (commandQueue[0] == "jobs")
    {
      printProcessList(processList);
      return exitNum;
    }
    else if (commandQueue[0] == "fg")
    {
      string argv = commandQueue[1];
      argv.erase(argv.begin());
      int id = atoi(argv.c_str());
      return movingProcessFGandBG((pid_t) id, true);
    }
    else if (commandQueue[0] == "bg")
    {
      string argv = commandQueue[1];
      argv.erase(argv.begin());
      int id = atoi(argv.c_str());
      return movingProcessFGandBG((pid_t) id, false);
    }

    if (commandQueue.back() == "&")
    {
      commandQueue.pop_back();
      toBackground = true;
    }

    pid_t pid = fork();
    int status;
    if (pid < 0)
    {
      perror("Fork failed");
      exit(errno);
    }
    else if (pid == 0)
    {
      setpgid(0,0);
      // Reference: https://stackoverflow.com/questions/35247570/execvp-not-working-when-converting-from-vectorstring-to-vectorchar-to-char
      vector<char*> argc;
      for (auto const &a : commandQueue)
        argc.emplace_back(const_cast<char*>(a.c_str()));
      argc.push_back(nullptr);
      /* This is the child, so execute the ls */ 
      exitNum = execvp(argc[0], argc.data());
      if (exitNum < 0)
      {
        cout << "Bad Command!" << endl;
        return exitNum;
      }
    }
    else
    {
      ProcessNode pnode;
      pnode.pid = pid;
      pnode.state = "Running";
      string str;
      for (auto const& s : commandQueue) { str += s; str += " ";}
      pnode.command = str;
      processList.push_back(pnode);

      if (toBackground)
      {
        setpgid(pid,pid);
        bgProcessHandler();
      }
      else
      {
        terminalState = false;
        parentProcessHandler(pid);
        terminalState = true;
      }
    } while (pid == 0);
    return exitNum;
}