/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_flow.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:54:02 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/25 09:35:07 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libft.h"
#include "exec.h"
#include "env/env.h"
#include "handler/handler.h"

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

static void	_close_fds(int32_t fds[2])
{
	if (fds && fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds && fds[1] != STDOUT_FILENO)
		close(fds[1]);
}

static void	_close_all_fds(int32_t fds[2])
{
	_close_fds(fds);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
}

static void	_print_cmd_err(char *cmd, int32_t fds[2])
{
	int32_t	errno_;

	errno_ = errno;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (fds)
		ft_putendl_fd("command not found", STDERR_FILENO);
	else
		ft_putendl_fd(strerror(errno_), STDERR_FILENO);
	_close_all_fds(fds);
}

static void	_exec_flow_cmd_cmd(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	char			**argv;
	char			**envp;
	char			*cmd;
	struct	stat	st;

	argv = &node->u_data.cmd.args->expanded;
	node->u_data.cmd.args = NULL;
	cmd = exec_get_cmd_path(argv, ctx);
	if (!cmd)
		return (_close_all_fds(fds), ft_splitfree(argv, 0), context_free(ctx),
			exit(1));
	if (!ft_strchr(cmd, '/') || access(cmd, F_OK) || !ft_strcmp(cmd, ".") || !ft_strcmp(cmd, ".."))
		return (_print_cmd_err(cmd, fds), free(cmd), ft_splitfree(argv, 0),
			context_free(ctx), exit(127));
	envp = env_get_envp(ctx->env, cmd, false);
	if (!envp)
		return (_close_all_fds(fds), ft_splitfree(argv, 0), context_free(ctx),
			free(cmd), exit(1));
	_dup_fds(fds);
	ast_free(ctx->head);
	ctx->head = NULL;
	execve(cmd, argv, envp);
	if (!stat(cmd, &st) && S_ISDIR(st.st_mode))
		errno = EISDIR;
	return (_print_cmd_err(cmd, NULL), free(cmd), ft_splitfree(argv, 0),
		ft_splitfree(envp, 0), context_free(ctx), exit(126));
}

static void	_exec_flow_cmd_builtin(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	char			**av;
	int32_t			old_fds[2];
	int32_t			ac;
	uint8_t			ret;

	sig_init(SIGH_RUNNING_BI);
	if (node->u_data.cmd.exec_infos.pid == 0)
		node->u_data.cmd.exec_infos.pid = -2;
	av = &node->u_data.cmd.args->expanded;
	ac = 0;
	while (av[ac])
		ac++;
	old_fds[0] = dup(STDIN_FILENO);
	old_fds[1] = dup(STDOUT_FILENO);
	_dup_fds(fds);
	ret = node->u_data.cmd.bi(ac, av, ctx);
	if (node->u_data.cmd.exec_infos.pid == -2)
		return (node->u_data.cmd.exec_infos.ret = ret, _dup_fds(old_fds));
	return (_close_all_fds(old_fds), context_free(ctx), exit(ret));
}

void	exec_flow_cmd(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	if (node && node->type == ND_REDIR)
		exec_flow_redir(ctx, node, fds);
	else if (node && node->type == ND_CMD && node->u_data.cmd.bi)
		_exec_flow_cmd_builtin(ctx, node, fds);
	else if (node && node->type == ND_CMD)
		_exec_flow_cmd_cmd(ctx, node, fds);
}
