#include "signalProcess.h"
#include "processHandler.h"
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <stddef.h>


using namespace std;

bool terminalState = true;

void pausedProcess(pid_t pid)
{
    cout << endl;
    if (pid == 0)
        return;
    printf("pid %d paused\n",pid);
    kill(pid, SIGSTOP);
}

void resumeProcess(pid_t pid)
{
    cout << endl;
    if (pid == 0)
        return;
    printf("pid %d resumed\n",pid);
    kill(pid, SIGCONT);
}

void killProcess(pid_t pid)
{
  cout << endl;
  printf("kill pid %d\n",pid);
  kill(pid, SIGKILL);
}

void sigChildHandler(int signal){
    pid_t pid;
    for (int i = 0; i < processList.size(); i++)
    {
        ProcessNode pnode = processList[i];
        if ((pid = waitpid(pnode.pid,0,WNOHANG)) > 0){
            cout << "Process " << pnode.pid <<" has finished executing" << endl; 
            processList.erase(processList.begin()+i);
            break;
        }
    }
    tcsetpgrp(STDIN_FILENO, shellID);
    tcsetpgrp(STDOUT_FILENO, shellID);
    tcsetpgrp(STDERR_FILENO, shellID);
    childID = 0;
}
