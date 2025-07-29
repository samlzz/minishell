/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_flow.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:54:02 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/29 04:31:02 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libft.h"
#include "exec.h"
#include "env/env.h"
#include "handler/handler.h"

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
	ft_close_all(fds);
}

static void	_exec_flow_cmd_cmd_exec(t_sh_ctx *ctx, char *cmd, char **argv,
																	char **envp)
{
	struct stat	st;

	ast_free(ctx->head);
	ctx->head = NULL;
	execve(cmd, argv, envp);
	if (!stat(cmd, &st) && S_ISDIR(st.st_mode))
		errno = EISDIR;
	return (_print_cmd_err(cmd, NULL), free(cmd), ft_splitfree(argv, 0),
		ft_splitfree(envp, 0), context_free(ctx), exit(126));
}

static void	_exec_flow_cmd_cmd(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	char	**argv;
	char	**envp;
	char	*cmd;

	argv = &node->u_data.cmd.args->expanded;
	node->u_data.cmd.args = NULL;
	cmd = exec_get_cmd_path(argv, ctx);
	if (!cmd)
		return (ft_close_all(fds), ft_splitfree(argv, 0), context_free(ctx),
			exit(1));
	if (!ft_strchr(cmd, '/') || access(cmd, F_OK) || !ft_strcmp(cmd, ".")
		|| !ft_strcmp(cmd, ".."))
		return (_print_cmd_err(cmd, fds), free(cmd), ft_splitfree(argv, 0),
			context_free(ctx), exit(127));
	envp = env_get_envp(ctx->env, cmd, false);
	if (!envp)
		return (ft_close_all(fds), ft_splitfree(argv, 0), context_free(ctx),
			free(cmd), exit(1));
	ft_dup_fds(fds);
	return (_exec_flow_cmd_cmd_exec(ctx, cmd, argv, envp));
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
	ft_dup_fds(fds);
	ret = node->u_data.cmd.bi(ac, av, ctx);
	if (node->u_data.cmd.exec_infos.pid == -2)
		return (node->u_data.cmd.exec_infos.ret = ret, ft_dup_fds(old_fds));
	return (ft_close_all(old_fds), context_free(ctx), exit(ret));
}

void	exec_flow_cmd(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	int32_t	ret;

	if (node && node->type == ND_REDIR)
		exec_flow_redir(ctx, node, fds);
	else if (node && node->type == ND_CMD && node->u_data.cmd.bi)
		_exec_flow_cmd_builtin(ctx, node, fds);
	else if (node && node->type == ND_CMD)
		_exec_flow_cmd_cmd(ctx, node, fds);
	else if (node && node->type == ND_SUBSHELL)
	{
		ft_dup_fds(fds);
		ret = exec_flow_exec(ctx, node->u_data.subsh.child,
				(int32_t[2]){STDIN_FILENO, STDOUT_FILENO});
		return (ft_close_all(NULL), context_free(ctx), exit(ret));
	}
}
