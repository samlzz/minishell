# BUG TO FIX:

### Case 1

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

cmd: `export TEST=~:"jsp"`

> minishell expand tild when it shouldn't

```sh
minishell> echo ~/"jsp"
/home/sliziard/jsp
Exit code: 0
├── CMD: echo /home/sliziard/jsp
minishell> echo ~/jsp
/home/sliziard/jsp
Exit code: 0
├── CMD: echo /home/sliziard/jsp
minishell> echo ~:jsp
/home/sliziard:jsp
Exit code: 0
├── CMD: echo /home/sliziard:jsp
minishell> echo ~:"jsp"
~:jsp
Exit code: 0
├── CMD: echo ~:jsp
minishell> export TEST=~/"jsp"
minishell: export: command not found: No such file or directory
Exit code: 127
├── CMD: export TEST=/home/sliziard/jsp
minishell> export TEST=~/jsp
minishell: export: command not found: No such file or directory
Exit code: 127
├── CMD: export TEST=/home/sliziard/jsp
minishell> export TEST=~:jsp
minishell: export: command not found: No such file or directory
Exit code: 127
├── CMD: export TEST=/home/sliziard:jsp
minishell> export TEST=~:"jsp"
minishell: export: command not found: No such file or directory
Exit code: 127
├── CMD: export TEST=/home/sliziard:jsp
```

---
