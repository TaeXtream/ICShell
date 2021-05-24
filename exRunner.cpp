#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <cassert>
#include <deque>
#include <vector>



using namespace std;

pid_t childID;
static int run = 1;
static int paused = 0;


void suspend_process(pid_t pid)
{
    if (pid == 0)
        return;
    printf("pid %d suspended\n",pid);
    paused = 1;
    kill(pid, SIGSTOP);
}

void resume_process(pid_t pid)
{
    if (pid == 0)
        return;
    printf("pid %d resumed\n",pid);
    paused = 0;
    kill(pid, SIGCONT);
}

void sigINThandler(int signal)
{
  cout << endl;
  printf("kill pid %d\n",childID);
  run = 0;
  kill(childID, SIGKILL);
}

void sigSTOPhandler(int signal)
{
  if(paused)
    resume_process(childID);
  else
    suspend_process(childID);
}

int runExternalCommand(deque<string> commandList)
{
  assert(commandList.size() > 0);
  int exitNum = 0;
  int status;
  pid_t pid;
  if ((pid=fork()) < 0)
  {
    perror ("Fork failed");
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
    exsaSTOP.sa_handler = &sigSTOPhandler;
    exsaINT.sa_handler = &sigINThandler;

    childID = pid;

    while (run && waitpid(pid, &status, WNOHANG) == 0)
    { 
      sigaction(SIGINT,&exsaINT, nullptr); 
      sigaction(SIGTSTP,&exsaSTOP, nullptr);
    }
  }
  return exitNum;
}

