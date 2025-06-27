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
