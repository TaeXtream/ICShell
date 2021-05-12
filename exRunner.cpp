#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <cassert>
#include <deque>
#include <vector>


using namespace std;

void runExternalCommand(deque<string> commandList)
{
  assert(commandList.size() > 0);
  int status;
  int pid;
  if ((pid=fork()) < 0)
  {
    perror ("Fork failed");
    exit(errno);
  }
  if (!pid)
  {
    // Reference: https://stackoverflow.com/questions/35247570/execvp-not-working-when-converting-from-vectorstring-to-vectorchar-to-char
    vector<char*> argc;
    for (auto const& a : commandList)
        argc.emplace_back(const_cast<char*>(a.c_str()));
    argc.push_back(nullptr);
    /* This is the child, so execute the ls */ 
    execvp(argc[0], argc.data());
  }

  if (pid)
  {
    /* 
    * We're in the parent; let's wait for the child to finish
    */
   waitpid (pid, NULL, 0);
  }
}


int main(int argc, char *argv[])
{
  deque<string> command;
  command.push_back("/usr/bin/ls");
  command.push_back("-l");
  command.push_back("/");
  runExternalCommand(command);
}
