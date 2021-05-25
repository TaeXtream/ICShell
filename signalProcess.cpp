#include "signalProcess.h"

#include <signal.h>
#include <iostream>
#include <unistd.h>

using namespace std;

void mainSignalHandler(int signal)
{
    cout << endl;
}

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

