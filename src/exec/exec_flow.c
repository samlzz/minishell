/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_flow.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:18:54 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/27 08:07:59 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "exec.h"
#include "builtins/builtins.h"
#include "handler/handler.h"
#include "env/env.h"

static t_expr	_exec_wait_get_exec_infos(t_ast *node)
{
	t_expr	left;

	if (node->type == ND_PIPE)
	{
		left = _exec_wait_get_exec_infos(node->u_data.op.left);
		if (left.pid == -1 && left.ret == 129)
			return (left);
		return (_exec_wait_get_exec_infos(node->u_data.op.right));
	}
	else if (node->type == ND_SUBSHELL)
		return (node->u_data.subsh.exec_infos);
	else if (node->type == ND_REDIR && node->u_data.rd.child)
		return (_exec_wait_get_exec_infos(node->u_data.rd.child));
	else if (node->type == ND_REDIR)
		return (node->u_data.rd.exec_infos);
	else if (node->type == ND_CMD)
		return (node->u_data.cmd.exec_infos);
	return ((t_expr){-1, -1, -1});
}

static void	_update_underscore(t_sh_ctx *ctx, t_ast *node)
{
	char	**argv;
	size_t	i;

	if (!node || (node->type != ND_CMD && node->type != ND_REDIR))
		return ;
	if (node->type == ND_REDIR)
		return (_update_underscore(ctx, node->u_data.rd.child));
	argv = &node->u_data.cmd.args->expanded;
	i = 0;
	while (argv[i])
		i++;
	if (i)
		env_literal_set(ctx->env, "_", argv[--i]);
	else
		env_literal_set(ctx->env, "_", "");
}

static uint8_t	_exec_wait(t_sh_ctx *ctx, t_ast *node)
{
	size_t	i;
	int32_t	status;
	pid_t	pid;
	uint8_t	ret;
	t_expr	infos;

	i = exec_wait_get_count(node, false);
	while (i-- > 0)
	{
		pid = wait(&status);
		ret = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			ret = 128 + WTERMSIG(status);
		exec_wait_set_ret(node, pid, ret, status);
	}
	_update_underscore(ctx, node);
	infos = _exec_wait_get_exec_infos(node);
	if (WIFSIGNALED(infos.status))
		print_sig_message(WTERMSIG(infos.status), WCOREDUMP(infos.status));
	return (infos.ret);
}

uint8_t	exec_flow_exec(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	if (node->type == ND_AND || node->type == ND_OR)
	{
		ctx->lst_exit = exec_flow_exec(ctx, node->u_data.op.left, fds);
		if (ctx->lst_exit != 130 && !ctx->exit
			&& ((node->type == ND_AND && !ctx->lst_exit)
				|| (node->type == ND_OR && ctx->lst_exit)))
			return (exec_flow_exec(ctx, node->u_data.op.right, fds));
		return (ctx->lst_exit);
	}
	else
	{
		if (!set_builtin_func(ctx, node))
			return (1);
		else if (is_builtin(node))
			exec_flow_cmd(ctx, node, fds);
		else
			exec_flow_pipe(ctx, node, (int32_t[3]){fds[0], fds[1], -1});
	}
	ctx->lst_exit = _exec_wait(ctx, node);
	g_sig = 0;
	return (ctx->lst_exit);
}

uint8_t	exec_wrapper(t_sh_ctx *ctx, t_ast *node)
{
	uint8_t			ret;
	struct termios	tc_in;
	struct termios	tc_out;

	ctx->head = node;
	if (isatty(STDIN_FILENO))
		tcgetattr(STDIN_FILENO, &tc_in);
	if (isatty(STDOUT_FILENO))
		tcgetattr(STDOUT_FILENO, &tc_out);
	ret = exec_flow_exec(ctx, node, (int32_t[2]){STDIN_FILENO, STDOUT_FILENO});
	sig_init(SIGH_MAIN);
	g_sig = 0;
	if (isatty(STDIN_FILENO))
		tcsetattr(STDIN_FILENO, TCSANOW, &tc_in);
	if (isatty(STDOUT_FILENO))
		tcsetattr(STDOUT_FILENO, TCSANOW, &tc_out);
	ctx->head = NULL;
	return (ret);
}
