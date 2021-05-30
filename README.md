# ICShell Project

Linux Shell written in C++ for ICCS227 project.

## Features:

#### Basic Stuff:

`echo <text>` command for printing text to screen.

```shell
icsh> echo Hello To the Shell
Hello To the Shell
```

`exit <exitcode>` command for exit the shell.

```shell
icsh> exit 1
Bye
```

`!!` command to repeat previous command.

```shell
icsh> echo hello world
hello world
icsh> !!
hello world
```

#### Script Mode:

Shell can read file that come from the argument for input.

```shell
## test.sh

echo hello
echo world
!!
exit 5
```

```shell
$ ./icsh test.sh
hello
world
world
Bye
```

#### Run Other Basic Linux Command:

Shell is supporting external Linux command using Linux system call like `fork()`, `execvp()`and etc.

```shell
icsh> ls -l
total 564
drwxrwxrwx 1 taeza007hd taeza007hd    512 May 30 19:00 Games
-rwxrwxrwx 1 taeza007hd taeza007hd  35149 May  8 10:58 LICENSE
-rwxrwxrwx 1 taeza007hd taeza007hd    281 May 29 14:00 Makefile
-rwxrwxrwx 1 taeza007hd taeza007hd   1004 May 30 20:14 README.md
-rwxrwxrwx 1 taeza007hd taeza007hd 141968 May 30 20:14 icsh
-rwxrwxrwx 1 taeza007hd taeza007hd   7835 May 30 20:07 icsh.cpp
-rwxrwxrwx 1 taeza007hd taeza007hd 155856 May 30 20:14 icsh.o
-rwxrwxrwx 1 taeza007hd taeza007hd     58 May 24 20:34 in.txt
-rwxrwxrwx 1 taeza007hd taeza007hd    758 May 25 09:06 out.txt
-rwxrwxrwx 1 taeza007hd taeza007hd   5184 May 30 18:26 processHandler.cpp
-rwxrwxrwx 1 taeza007hd taeza007hd    342 May 29 21:18 processHandler.h
-rwxrwxrwx 1 taeza007hd taeza007hd 160856 May 30 20:14 processHandler.o
-rwxrwxrwx 1 taeza007hd taeza007hd   1091 May 30 18:26 signalProcess.cpp
-rwxrwxrwx 1 taeza007hd taeza007hd    270 May 30 18:26 signalProcess.h
-rwxrwxrwx 1 taeza007hd taeza007hd  41800 May 30 20:14 signalProcess.o
-rwxrwxrwx 1 taeza007hd taeza007hd     34 May  8 15:37 test.sh
```

```shell
icsh> ps -la
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 S  1000   401   400  0  80   0 -   652 do_wai pts/1    00:00:00 sh
0 S  1000   407   401  0  80   0 -   652 do_wai pts/1    00:00:00 sh
0 S  1000   409   407  0  80   0 - 251221 -     pts/1    00:00:24 node
0 S  1000   420   409  0  80   0 - 153330 -     pts/1    00:00:01 node
0 S  1000   551   409  0  80   0 - 220218 -     pts/1    00:00:09 node
0 S  1000   750   409  1  80   0 - 333840 -     pts/1    00:03:30 node
0 S  1000   769   750  0  80   0 - 556660 -     pts/1    00:01:15 cpptools
0 S  1000  2774   769  0  80   0 - 1386775 futex_ pts/1  00:00:02 cpptools-srv
0 S  1000  3638   769  0  80   0 - 1386494 futex_ pts/1  00:00:01 cpptools-srv
0 R  1000  3687  3255  0  80   0 -  1486 -      pts/0    00:00:00 icsh
0 R  1000  3689  3687  0  80   0 -  2634 -      pts/0    00:00:00 ps
```

#### Signal Handler:

Shell is handling Ctrl-Z and Ctrl-C signals for using with shell process.

1. Ctrl-Z generates a **SIGTSTP**. 
   For pause and resume current running process.
2. Ctrl-C generates a **SIGINT**.
   For kill current running process.

```shell
icsh> sleep 100s
^Z
pid 3724 paused
^Z
pid 3724 resumed
^C
kill pid 3724
icsh> exit
Bye
```

#### I/O redirection:

You can redirect out put and input of the shell by using > and <.

`< input.txt` Redirect shell to take input from input.txt.

`echo Go to the file > output.txt` Redirect output of echo command to output.txt.

`< input.txt > output.txt` Redirect shell to take input from input.txt and write shell output to output.txt.

#### Background jobs and job control:

I hope this feature work well~

You can control shell jobs.

`jobs` command for looking at background process list.

Use `<process command> &` to run process in background.

```shell
icsh> sleep 100 &
icsh> sleep 200 &
icsh> jobs
[0] 3732 Running sleep 100
[1] 3733 Running sleep 200
```

`fg %<pid>` Command for bring background process to foreground.

```shell
icsh> sleep 10s &
icsh> jobs
[0] 4554 Running sleep 10s
icsh> fg %4554
sleep 10s
```

`<bg %<pid>` Command for bring foreground process to background.

```shell
icsh> sleep 15
^Z
pid 4752 paused
icsh> jobs
[0] 4752 Paused sleep 15
icsh> bg %4752
icsh> Process 4752 has finished executing
```



## Extra Feature:

`game <Game Name>` Command to play some game might not work in your machine but it work in mine.

`matrix` Command to enter the Matrix.

This

References:

https://danishpraka.sh/2018/01/15/write-a-shell.html

https://www.geeksforgeeks.org/making-linux-shell-c/

https://brennan.io/2015/01/16/write-a-shell-in-c/

https://github.com/danishprakash/dash

https://github.com/hungys/mysh

https://www.youtube.com/watch?v=jF-1eFhyz1U&t=3s

https://www.youtube.com/watch?v=8OK8_tHeCIA&list=PLrOv9FMX8xJE8NgepZR1etrsU63fDDGxO
