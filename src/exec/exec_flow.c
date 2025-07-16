/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_flow.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:18:54 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/18 11:30:51 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "exec/exec.h"
#include "minishell.h"

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
	return ((t_expr){-1, -1});
}

static void	_exec_wait_set_ret(t_ast *node, pid_t pid, uint8_t ret)
{
	if (node && node->type == ND_PIPE)
	{
		_exec_wait_set_ret(node->u_data.op.left, pid, ret);
		_exec_wait_set_ret(node->u_data.op.right, pid, ret);
	}
	else if (node && node->type == ND_REDIR
		&& node->u_data.rd.exec_infos.pid == pid)
		node->u_data.rd.exec_infos.ret = ret;
	else if (node && node->type == ND_REDIR)
		_exec_wait_set_ret(node->u_data.rd.child, pid, ret);
	else if (node && node->type == ND_SUBSHELL
		&& node->u_data.subsh.exec_infos.pid == pid)
		node->u_data.subsh.exec_infos.ret = ret;
	else if (node && node->type == ND_CMD && node->u_data.cmd.exec_infos.pid == pid)
		node->u_data.cmd.exec_infos.ret = ret;
}

size_t	exec_wait_get_count(t_ast *node, bool is_hd)
{
	if (!node)
		return (0);
	if (node->type == ND_PIPE || (is_hd && 
		(node->type == ND_AND || node->type == ND_OR)))
		return (exec_wait_get_count(node->u_data.op.left, is_hd)
			+ exec_wait_get_count(node->u_data.op.right, is_hd));
	else if (node->type == ND_SUBSHELL)
		return (node->u_data.subsh.exec_infos.pid > 0);
	else if (node->type == ND_REDIR)
	{
		if (node->u_data.rd.child && is_hd)
			return (
				exec_wait_get_count(node->u_data.rd.child, is_hd) +
				node->u_data.rd.exec_infos.pid > 0
			);
		else if (node->u_data.rd.child)
			return (exec_wait_get_count(node->u_data.rd.child, is_hd));
		else
			return (node->u_data.rd.exec_infos.pid > 0);
	}
	else if (node->type == ND_CMD)
		return (node->u_data.cmd.exec_infos.pid > 0);
	return (0);
}

static void	_update_underscore(t_sh_ctx *ctx, t_ast *node)
{
	t_token	*errtok;
	char	**argv;
	size_t	i;

	if (!node || (node->type != ND_CMD && node->type != ND_REDIR))
		return ;
	if (node->type == ND_REDIR)
		return (_update_underscore(ctx, node->u_data.rd.child));
	errtok = NULL;
	if (expand_node(ctx, node, &errtok))
		return ;
	argv = &node->u_data.cmd.args->expanded;
	i = 0;
	while (argv[i])
		i++;
	if (i)
		env_literal_set(&ctx->env, "_", argv[--i]);
	else
		env_literal_set(&ctx->env, "_", "");
}

static uint8_t	_exec_wait(t_sh_ctx *ctx, t_ast *node)
{
	size_t	i;
	int32_t	status;
	pid_t	pid;
	uint8_t	ret;

	i = exec_wait_get_count(node, false);
	while (i-- > 0)
	{
		pid = wait(&status);
		ret = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
			ret = 128 + WTERMSIG(status);
		_exec_wait_set_ret(node, pid, ret);
	}
	_update_underscore(ctx, node);
	return (_exec_wait_get_exec_infos(node).ret);
}

#ifdef DEBUG_MODE

uint8_t	exec_flow_exec(t_sh_ctx *ctx, t_ast *root, t_ast *node, int32_t fds[2])
{
	uint8_t	ret;

	if (node->type == ND_AND || node->type == ND_OR)
	{
		ctx->lst_exit = exec_flow_exec(ctx, root, node->u_data.op.left, fds);
		if (ctx->lst_exit != 130 && !ctx->exit && (
			(node->type == ND_AND && !ctx->lst_exit) ||
			(node->type == ND_OR && ctx->lst_exit)
		))
			return (exec_flow_exec(ctx, root, node->u_data.op.right, fds));
		return (ctx->lst_exit);
	}
	else
	{
		ret = is_builtin(ctx, root, node);
		if (ret == 2)
			return (1);
		else if (ret == 1)
			exec_flow_cmd(ctx, root, node, fds);
		else
			exec_flow_pipe(ctx, root, node, (int32_t[3]){fds[0], fds[1], -1});
	}
	ctx->lst_exit = _exec_wait(ctx, node);
	g_sig = 0;
	if (PRINT_EXIT_CODE)
		dprintf(2, "Exit code: %d\n", ctx->lst_exit);
	return (ctx->lst_exit);
}
#else

uint8_t	exec_flow_exec(t_sh_ctx *ctx, t_ast *root, t_ast *node, int32_t fds[2])
{
	if (node->type == ND_AND || node->type == ND_OR)
	{
		ctx->lst_exit = exec_flow_exec(ctx, root, node->u_data.op.left, fds);
		if (ctx->lst_exit != 130 && (
			(node->type == ND_AND && !ctx->lst_exit) ||
			(node->type == ND_OR && ctx->lst_exit)
		))
			return (exec_flow_exec(ctx, root, node->u_data.op.right, fds));
		return (ctx->lst_exit);
	}
	else
		exec_flow_pipe(ctx, root, node, (int32_t[3]){fds[0], fds[1], -1});
	ctx->lst_exit = _exec_wait(ctx, node);
	g_sig = 0;
	return (ctx->lst_exit);
}
#endif

uint8_t	exec_wrapper(t_sh_ctx *ctx, t_ast *node)
{
	uint8_t			ret;
	struct termios	tc_in;
	struct termios	tc_out;
	struct termios	tc_err;

	if (tcgetattr(STDIN_FILENO, &tc_in) == -1)
		perror("minishell: tcgetattr");
	if (tcgetattr(STDOUT_FILENO, &tc_out) == -1)
		perror("minishell: tcgetattr");
	if (tcgetattr(STDERR_FILENO, &tc_err) == -1)
		perror("minishell: tcgetattr");
	ret = exec_flow_exec(ctx, node, node, (int32_t[2]){STDIN_FILENO,
		STDOUT_FILENO});
	sig_init(SIGH_MAIN);
	g_sig = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &tc_in) == -1)
		perror("minishell: tcsetattr");
	if (tcsetattr(STDOUT_FILENO, TCSANOW, &tc_out) == -1)
		perror("minishell: tcsetattr");
	if (tcsetattr(STDERR_FILENO, TCSANOW, &tc_err) == -1)
		perror("minishell: tcsetattr");
	return (ret);
}
