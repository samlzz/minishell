# BUG TO FIX:

### Test 1

cmd: `echo $USER $123456789USER $USER123456789`

> minishell add a space at the eol

---

### Case 2

cmd: `~root`

> minishell return :

```sh
├── CMD: echo hey /home/sliziard
minishell> ~root
minishell: /root: Permission denied
Exit code: 126
```

> bash return:

```sh
bash-5.2$ ~root
bash: /root: Is a directory
```

---

### Case 3

cmd: `minishell && echo test`
_when command not found_

> minishell doesn't close 1 fd

**Valgrind output**

```
minishell> minishell && echo test
minishell: minishell: command not found
==86616==
==86616== FILE DESCRIPTORS: 3 open (2 inherited) at exit.
==86616== Open file descriptor 1: /dev/pts/0
==86616==    at 0x49EEA2B: dup2 (dup2.c:29)
==86616==    by 0x40024DA: _print_cmd_err (in /home/sliziard/42/minishell/minishell)
==86616==    by 0x40026D1: _exec_flow_cmd_cmd (in /home/sliziard/42/minishell/minishell)
==86616==    by 0x4002880: exec_flow_cmd (in /home/sliziard/42/minishell/minishell)
==86616==    by 0x4003409: exec_flow_pipe (in /home/sliziard/42/minishell/minishell)
==86616==    by 0x4003139: exec_flow_exec (in /home/sliziard/42/minishell/minishell)
==86616==    by 0x40030AA: exec_flow_exec (in /home/sliziard/42/minishell/minishell)
==86616==    by 0x40031B6: exec_wrapper (in /home/sliziard/42/minishell/minishell)
==86616==    by 0x4001444: _launch_exec (in /home/sliziard/42/minishell/minishell)
==86616==    by 0x40015D9: main (in /home/sliziard/42/minishell/minishell)
```
