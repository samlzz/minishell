/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 21:23:14 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/06 08:05:42 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast/ast.h"
#include "env/env.h"
#include "exec/exec.h"

void	print_ast_ascii(t_ast *node);
int32_t	exec_node_flow_exec(t_hmap *envp, t_ast *root, t_ast *node, int32_t fds[2]);

static inline char	*_concat_path_cmd(char *path, char *cmd)
{
	size_t	len;
	char	*ret;

	len = ft_strlen(path) + ft_strlen(cmd) + 2;
	ret = ft_calloc(len, sizeof(char));
	if (!ret)
		return (perror("exec: _concat_path_cmd: malloc"), NULL);
	ft_strlcat(ret, path, len);
	ft_strlcat(ret, "/", len);
	ft_strlcat(ret, cmd, len);
	return (ret);
}

static inline char	*_cmd_finder(char *cmd, char **path)
{
	size_t	i;
	char	*tmp;
	char	*f;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/') || ft_strlen(cmd) == 0)
		return (ft_strdup(cmd));
	f = NULL;
	i = -1;
	while (path[++i])
	{
		tmp = _concat_path_cmd(path[i], cmd);
		if (!tmp)
			return (free(f), NULL);
		if (!f && access(tmp, F_OK) != -1)
			f = tmp;
		if (access(tmp, X_OK) != -1)
			return (free((void *)((f != tmp) * (uintptr_t)(f))), tmp);
		else if (f != tmp)
			free(tmp);
	}
	if (f)
		return (f);
	return (free(f), ft_strdup(cmd));
}

static inline char	*_get_cmd_path(char **av, t_hmap *envp)
{
	char	**path;
	char	*path_str;
	char	*cmd;


	path_str = ft_hmap_get(envp, "PATH");
	if (!path_str)
		return (ft_calloc(1, sizeof(char *)));
	path = ft_split(path_str, ':');
	if (!path)
		return (perror("exec: _get_cmd_path: ft_split"), NULL);
	cmd = _cmd_finder(av[0], path);
	ft_splitfree(path, 0);
	return (cmd);
}

char	**dup_argv(char **av)
{
	char	**ret;
	size_t	ac;
	size_t	i;

	ac = 0;
	while (av[ac])
		ac++;
	ret = ft_calloc(ac + 1, sizeof(char *));
	if (!ret)
		return (perror("exec: dup_argv: malloc"), NULL);
	i = -1;
	while (++i < ac)
	{
		ret[i] = ft_strdup(av[i]);
		if (!ret[i])
			return (perror("exec: dup_argv: ft_strdup"), ft_splitfree(ret, 0),
				NULL);
	}
	return (ret);
}

pid_t	exec_node_flow_command(t_hmap *envp, t_ast *root, t_ast *node, int32_t fds[3])
{
	pid_t	pid;
	int32_t	fd;
	int32_t	oflag;
	char	*cmd;
	char	**argv;

	printf("flow command\n");
	print_ast_ascii(node);
	if (node->type == ND_CMD)
	{
		pid = fork();
		if (pid == -1)
			return (perror("exec: exec_node_flow_command: fork"),
				ast_free(root), -1);
		else if (pid == 0)
		{
			if (dup2(fds[0], STDIN_FILENO) == -1
				|| dup2(fds[1], STDOUT_FILENO) == -1)
				return (perror("exec: exec_node_flow_command: dup2"),
					ast_free(root), -1);
			if (fds[0] != STDIN_FILENO)
				close(fds[0]);
			if (fds[1] != STDOUT_FILENO)
				close(fds[1]);
			if (fds[2] != -1)
				close(fds[2]);
			argv = dup_argv(node->u_data.s_cmd.argv);
			if (!argv)
				return (ast_free(root), -1);
			ast_free(root);
			cmd = _get_cmd_path(argv, envp);
			dprintf(2, "cmd: %s\n", cmd);
			if (!cmd)
				return (-1);
			if (access(cmd, F_OK))
				return (perror("Command not found"), free(cmd), 127);
			if (access(cmd, X_OK))
				return (perror("Command not executable"), free(cmd), 126);
			execve(cmd, argv, get_envp(envp));
			perror("exec: exec_node_flow_command: execve");
			exit(-1);
		}
	}
	else
	{
		if (node->u_data.rd.redir_type == RD_IN)
		{
			fd = open(node->u_data.rd.filename, O_RDONLY);
			if (fd == -1)
				return (perror("exec: exec_node_flow_command: open"),
					ast_free(root), -1);
			if (fds[0] != STDIN_FILENO)
				close(fds[0]);
			fds[0] = fd;
		}
		else if (node->u_data.rd.redir_type == RD_OUT || node->u_data.rd.redir_type == RD_APPEND)
		{
			oflag = O_WRONLY | O_CREAT | O_APPEND;
			if (node->u_data.rd.redir_type == RD_OUT)
				oflag = O_WRONLY | O_CLOEXEC | O_CREAT | O_TRUNC;
			fd = open(node->u_data.rd.filename, oflag, 0644);
			if (fd == -1)
				return (perror("exec: exec_node_flow_command: open"),
					ast_free(root), -1);
			if (fds[1] != STDOUT_FILENO)
				close(fds[1]);
			fds[1] = fd;
		}
		pid = exec_node_flow_command(envp, root, node->u_data.rd.child, fds);
	}
	return (pid);
}

pid_t	exec_node_flow_pipe(t_hmap *envp, t_ast *root, t_ast *node, int32_t fds[3])
{
	int32_t	fds_[2];
	pid_t	ret;

	printf("flow pipe\n");
	print_ast_ascii(node);
	if (node->type == ND_PIPE)
	{
		if (pipe(fds_) == -1)
			return (perror("exec: exec_node_flow_pipe"), ast_free(root), -1);
		exec_node_flow_pipe(envp, root, node->u_data.op.left,
				(int32_t[3]){fds[0], fds_[1], fds_[0]});
		close(fds_[1]);
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		ret = exec_node_flow_pipe(envp, root, node->u_data.op.right,
			(int32_t[3]){fds_[0], fds[1], -1});
		close(fds_[0]);
	}
	else if (node->type == ND_SUBSHELL)
		ret = exec_node_flow_pipe(envp, root, node->u_data.s_subsh.child, fds);
	else
		ret = exec_node_flow_command(envp, root, node, fds);
	return (ret);
}

int32_t	exec_node_wait(t_ast *node, pid_t last)
{
	int32_t	status;
	int32_t	ret;
	size_t	count;
	pid_t	pid;

	printf("wait\n");
	print_ast_ascii(node);
	count = 1;
	if (node->type == ND_PIPE)
		while (node && node->type == ND_PIPE && ++count)
			node = node->u_data.op.right;
	printf("wait count: %lu\n", count);
	printf("wait pid: %d\n", last);
	while (count--)
	{
		pid = wait(&status);
		if (pid == -1)
			return (perror("exec: exec_node_wait: wait"), -1);
		else if (pid == last && !WIFSIGNALED(status))
			ret = WEXITSTATUS(status);
		else if (pid == last && WIFSIGNALED(status))
			ret = 128 + WTERMSIG(status);
	}
	return (ret);
}

int32_t	exec_node_flow_exec(t_hmap *envp, t_ast *root, t_ast *node, int32_t fds[2])
{
	int32_t	ret;
	pid_t	pid;

	printf("flow exec\n");
	print_ast_ascii(node);
	if (node->type == ND_AND || node->type == ND_OR)
	{
		ret = exec_node_flow_exec(envp, root, node->u_data.op.left, fds);
		if ((node->type == ND_AND && !ret) || (node->type == ND_OR && ret))
			ret = exec_node_flow_exec(envp, root, node->u_data.op.right, fds);
	}
	else if (node->type == ND_SUBSHELL)
	{
		pid = fork();
		if (pid == -1)
			return (perror("exec: exec_node_flow_exec: fork"), -1);
		else if (pid == 0)
		{
			ret = exec_node_flow_exec(envp, root, node->u_data.s_subsh.child, fds);
			ast_free(root);
			exit(ret);
		}
		else
			ret = exec_node_wait(node, pid);
	}
	else
		ret = exec_node_wait(node, exec_node_flow_pipe(envp, root, node, fds));
	printf("exit code: %d\n", ret);
	return (ret);
}

void	exec_preprocess_ast_reverse_pipes(t_ast **root)
{
	t_ast	*cur;
	t_ast	*prev;
	t_ast	*next;

	prev = NULL;
	cur = *root;
	while (cur && cur->type == ND_PIPE)
	{
		next = cur->u_data.op.left;
		if (prev)
			cur->u_data.op.right = prev;
		if (next && next->type == ND_PIPE)
			cur->u_data.op.left = next->u_data.op.right;
		prev = cur;
		cur = next;
	}
	*root = prev;
}

void	exec_preprocess_ast(t_ast **root)
{
	if ((*root)->type == ND_PIPE)
		exec_preprocess_ast_reverse_pipes(root);
	else if ((*root)->type == ND_SUBSHELL)
		exec_preprocess_ast(&(*root)->u_data.s_subsh.child);
	else if ((*root)->type == ND_AND || (*root)->type == ND_OR)
	{
		exec_preprocess_ast(&(*root)->u_data.op.left);
		exec_preprocess_ast(&(*root)->u_data.op.right);
	}
}

void	exec_node_wrapper(t_hmap *envp, t_ast *node)
{
	exec_preprocess_ast(&node);
	print_ast_ascii(node);
	exec_node_flow_exec(envp, node, node, (int32_t[2]){STDIN_FILENO,
		STDOUT_FILENO});
}
