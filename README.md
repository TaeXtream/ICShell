# ICShell Project

Linux Shell written in C++ for ICCS227 project.

Features:

```echo``` command for printing text to screen.

```shell
icsh> echo Hello To the Shell
Hello To the Shell
```

`exit` command for exit the shell.

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

Script Mode:

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

References:

https://danishpraka.sh/2018/01/15/write-a-shell.html

https://www.geeksforgeeks.org/making-linux-shell-c/

https://brennan.io/2015/01/16/write-a-shell-in-c/
