/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_flow.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:18:54 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/20 08:19:00 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec/exec.h"

static uint8_t	_exec_wait_get_ret(t_ast *node)
{
	(void) node;
	return (0);
}

static void	_exec_wait_set_ret(t_ast *node, pid_t pid, uint8_t ret)
{
	if (node && node->type == ND_PIPE)
	{
		_exec_wait_set_ret(node->u_data.op.left, pid, ret);
		_exec_wait_set_ret(node->u_data.op.right, pid, ret);
	}
	else if (node && node->type == ND_REDIR)
		_exec_wait_set_ret(node->u_data.rd.child, pid, ret);
	else if (node && node->type == ND_SUBSHELL
		&& node->u_data.subsh.exec_infos.pid == pid)
		node->u_data.subsh.exec_infos.ret = ret;
	else if (node && node->type == ND_CMD && node->u_data.cmd.exec_infos.pid == pid)
		node->u_data.cmd.exec_infos.ret = ret;
}

static size_t	_exec_wait_get_count(t_ast *node)
{
	if (node && node->type == ND_PIPE)
		return (_exec_wait_get_count(node->u_data.op.left)
			+ _exec_wait_get_count(node->u_data.op.right));
	else if (node && node->type == ND_SUBSHELL)
		return (node && node->u_data.subsh.exec_infos.pid > 0);
	else if (node && node->type == ND_REDIR)
		return (_exec_wait_get_count(node->u_data.rd.child));
	else if (node && node->type == ND_CMD)
		return (node->u_data.cmd.exec_infos.pid > 0);
	return (0);
}

static uint8_t	_exec_wait(t_ast *node)
{
	size_t	i;
	int32_t	status;
	pid_t	pid;
	uint8_t	ret;

	i = _exec_wait_get_count(node);
	while (i-- > 0)
	{
		pid = wait(&status);
		ret = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			ret = 128 + WTERMSIG(status);
		_exec_wait_set_ret(node, pid, ret);
	}
	return (_exec_wait_get_ret(node));
}

uint8_t	exec_flow_exec(t_hmap *env, t_ast *root, t_ast *node, int32_t fds[2])
{
	uint8_t	ret;

	if (node->type == ND_AND || node->type == ND_OR)
	{
		ret = exec_flow_exec(env, root, node->u_data.op.left, fds);
		if ((node->type == ND_AND && !ret) || (node->type == ND_OR && ret))
			return (exec_flow_exec(env, root, node->u_data.op.right, fds));
		return (ret);
	}
	else if (node->type == ND_SUBSHELL)
	{
		node->u_data.subsh.exec_infos.pid = fork();
		if (node->u_data.subsh.exec_infos.pid == -1)
			return (perror("minishell: fork"),
				(node->u_data.subsh.exec_infos.ret = 254));
		else if (node->u_data.subsh.exec_infos.pid == 0)
		{
			ret = exec_flow_exec(env, root, node->u_data.subsh.child, fds);
			return (exit(ret), ret);
		}
	}
	else
		exec_flow_pipe(env, root, node, (int32_t[3]){fds[0], fds[1], -1});
	return (_exec_wait(node));
}

uint8_t	exec_wrapper(t_hmap *env, t_ast *node)
{
	uint8_t	ret;

	ret = exec_flow_exec(env, node, node, (int32_t[2]){STDIN_FILENO,
		STDOUT_FILENO});
	return (ret);
}
