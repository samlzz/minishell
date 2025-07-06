/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_flow.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:39:12 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/07 19:22:12 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "exec/exec.h"
#include "handler/handler.h"

static bool	_set_pipe_pid_ret(t_ast *node, pid_t pid, uint8_t ret)
{
	if (node && node->type == ND_PIPE)
		return (_set_pipe_pid_ret(node->u_data.op.left, pid, ret));
	else if (node && node->type == ND_REDIR
		&& !_set_pipe_pid_ret(node->u_data.rd.child, pid, ret))
	{
		node->u_data.rd.exec_infos.pid = pid;
		node->u_data.rd.exec_infos.ret = ret;
		return (true);
	}
	else if (node && node->type == ND_SUBSHELL)
	{
		node->u_data.subsh.exec_infos.pid = pid;
		node->u_data.subsh.exec_infos.ret = ret;
		return (true);
	}
	else if (node && node->type == ND_CMD)
	{
		node->u_data.cmd.exec_infos.pid = pid;
		node->u_data.cmd.exec_infos.ret = ret;
		return (true);
	}
	return (false);
}

void	exec_flow_pipe(t_sh_ctx *ctx, t_ast *root, t_ast *node, int32_t fds[3])
{
	int32_t	fds_[2];
	int32_t	ret;
	pid_t	pid;

	if (node->type != ND_PIPE)
	{
		sig_init(SIGH_RUNNING_CH);
		pid = fork();
		if (pid == -1)
			return (_set_pipe_pid_ret(node, -1, 254), perror("minishell: fork"));
		if (pid == 0)
		{
			if (fds[2] != -1)
				close(fds[2]);
			sig_init(SIGH_RESTORE);
			if (node->type == ND_SUBSHELL)
			{
				ret = exec_flow_exec(ctx, root, node->u_data.subsh.child, fds);
				if (fds[0] != STDIN_FILENO)
					close(fds[0]);
				if (fds[1] != STDOUT_FILENO)
					close(fds[1]);
				return (ast_free(root), context_free(ctx), exit(ret));
			}
			else
				return (_set_pipe_pid_ret(node, -1, 0),
					exec_flow_cmd(ctx, root, node, fds));
		}
		return ((void) _set_pipe_pid_ret(node, pid, 0));
	}
	if (pipe(fds_) == -1)
		return (_set_pipe_pid_ret(node, -1, 129), perror("minishell: pipe error"));
	exec_flow_pipe(ctx, root, node->u_data.op.left, (int32_t[3]){fds[0], fds_[1], fds_[0]});
	close(fds_[1]);
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	exec_flow_pipe(ctx, root, node->u_data.op.right, (int32_t[3]){fds_[0], fds[1], -1});
	close(fds_[0]);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
}
