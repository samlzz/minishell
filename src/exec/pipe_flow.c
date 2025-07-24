/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_flow.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 22:39:12 by mle-flem          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/07/24 15:41:38 by sliziard         ###   ########.fr       */
=======
/*   Updated: 2025/07/24 11:35:47 by sliziard         ###   ########.fr       */
>>>>>>> 59053d7 (feat!: add a MINISHELL_BONUS flag that compile bonus or not)
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "exec.h"
#include "env/env.h"
#include "handler/handler.h"

#ifdef MINISHELL_BONUS

static bool	_set_pipe_pid_ret(t_ast *node, pid_t pid, uint8_t ret)
{
	if (node && node->type == ND_PIPE)
		return (_set_pipe_pid_ret(node->u_data.op.left, pid, ret));
	else if (node && node->type == ND_REDIR)
	{
		if (_set_pipe_pid_ret(node->u_data.rd.child, pid, ret))
			return (true);
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

static void	_dup_err(int32_t oldfd, int32_t newfd)
{
	char str[61];

	str[0] = 0;
	ft_strlcat(str, "minishell: cannot duplicate fd ", 61);
	ft_itoa_str(str + ft_strlen(str), oldfd);
	ft_strlcat(str, " to fd ", 61);
	ft_itoa_str(str + ft_strlen(str), newfd);
	perror(str);
}

static void _dup_fds(int32_t fds[2])
{
	if (dup2(fds[1], STDOUT_FILENO) == -1)
		_dup_err(fds[1], STDOUT_FILENO);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	if (dup2(fds[0], STDIN_FILENO) == -1)
		_dup_err(fds[0], STDIN_FILENO);
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
}

void	exec_flow_pipe(t_sh_ctx *ctx, t_ast *node, int32_t fds[3])
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
				_dup_fds(fds);
				ret = exec_flow_exec(ctx, node->u_data.subsh.child,
						(int32_t[2]){STDIN_FILENO, STDOUT_FILENO});
				close(STDIN_FILENO);
				close(STDOUT_FILENO);
				return (context_free(ctx), exit(ret));
			}
			else
				return (_set_pipe_pid_ret(node, -1, 0),
					exec_flow_cmd(ctx, node, fds));
		}
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		return ((void) _set_pipe_pid_ret(node, pid, 0));
	}
	if (pipe(fds_) == -1)
		return (_set_pipe_pid_ret(node, -1, 129), perror("minishell: pipe error"));
	exec_flow_pipe(ctx, node->u_data.op.left, (int32_t[3]){fds[0], fds_[1], fds_[0]});
	exec_flow_pipe(ctx, node->u_data.op.right, (int32_t[3]){fds_[0], fds[1], -1});
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
}

#else

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
	else if (node && node->type == ND_CMD)
	{
		node->u_data.cmd.exec_infos.pid = pid;
		node->u_data.cmd.exec_infos.ret = ret;
		return (true);
	}
	return (false);
}

void	exec_flow_pipe(t_sh_ctx *ctx, t_ast *node, int32_t fds[3])
{
	int32_t	fds_[2];
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
			return (_set_pipe_pid_ret(node, -1, 0),
				exec_flow_cmd(ctx, node, fds));
		}
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		return ((void) _set_pipe_pid_ret(node, pid, 0));
	}
	if (pipe(fds_) == -1)
		return (_set_pipe_pid_ret(node, -1, 129), perror("minishell: pipe error"));
	exec_flow_pipe(ctx, node->u_data.op.left, (int32_t[3]){fds[0], fds_[1], fds_[0]});
	exec_flow_pipe(ctx, node->u_data.op.right, (int32_t[3]){fds_[0], fds[1], -1});
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
}

#endif