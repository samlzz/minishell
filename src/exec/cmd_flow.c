/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_flow.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:54:02 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/25 08:51:46 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "exec/exec.h"

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

static void	_print_cmd_err(char *cmd, int32_t fds[2])
{
	char	*str;
	size_t	len;
	int32_t	errno_;

	errno_ = errno;
	len = ft_strlen("minishell: ") + ft_strlen(cmd) + 1;
	if (fds)
		len += ft_strlen(": command not found");
	str = malloc(len);
	if (!str)
	{
		if (fds && fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds && fds[1] != STDOUT_FILENO)
			close(fds[1]);
		return (perror("minishell: malloc"));
	}
	str[0] = 0;
	ft_strlcat(str, "minishell: ", len);
	ft_strlcat(str, cmd, len);
	if (fds)
		ft_strlcat(str, ": command not found", len);
	errno = errno_;
	perror(str);
	free(str);
	if (fds && fds[0] != STDIN_FILENO)
		close(fds[0]);
	if (fds && fds[1] != STDOUT_FILENO)
		close(fds[1]);
}

static void	_exec_flow_cmd_cmd(t_sh_ctx *ctx, t_ast *root, t_ast *node, int32_t fds[2])
{
	char	**argv;
	char	**envp;
	t_token	*errtok;
	char	*cmd;

	errtok = NULL;
	if (expand_node(ctx, node, &errtok))
	{
		err_print_expand(errtok);
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		return (context_free(ctx), exit(1));
	}
	argv = &node->u_data.cmd.args->expanded;
	node->u_data.cmd.args = NULL;
	ast_free(root);
	envp = get_envp(&ctx->env);
	if (!envp)
	{
		perror("minishell: malloc");
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		return (ft_splitfree(argv, 0), context_free(ctx), exit(1));
	}
	cmd = exec_get_cmd_path(argv, &ctx->env);
	if (access(cmd, F_OK))
		return (_print_cmd_err(cmd, fds), free(cmd), ft_splitfree(argv, 0),
			ft_splitfree(envp, 0), context_free(ctx), exit(127));
	_dup_fds(fds);
	execve(cmd, argv, envp);
	return (_print_cmd_err(cmd, NULL), free(cmd), ft_splitfree(argv, 0),
		ft_splitfree(envp, 0), context_free(ctx), exit(126));
}

void	exec_flow_cmd(t_sh_ctx *ctx, t_ast *root, t_ast *node, int32_t fds[2])
{
	if (node && node->type == ND_REDIR)
		exec_flow_redir(ctx, root, node, fds);
	else if (node && node->type == ND_CMD)
		_exec_flow_cmd_cmd(ctx, root, node, fds);
}
