/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:53:56 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/25 10:58:32 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "builtins/builtins.h"
#include "exec/exec.h"
#include "parser/parser.h"

static pid_t	_get_node_pid(t_ast *node)
{
	if (node && node->type == ND_REDIR && node->u_data.rd.child)
		return (_get_node_pid(node->u_data.rd.child));
	else if (node && node->type == ND_REDIR)
		return (node->u_data.rd.exec_infos.pid);
	else if (node && node->type == ND_CMD)
		return (node->u_data.cmd.exec_infos.pid);
	else if (node && node->type == ND_SUBSHELL)
		return (node->u_data.subsh.exec_infos.pid);
	return (-1);
}

static bool	_set_node_ret(t_ast *node, uint8_t ret)
{
	if (node && node->type == ND_PIPE)
		return (_set_node_ret(node->u_data.op.left, ret));
	else if (node && node->type == ND_REDIR
		&& !_set_node_ret(node->u_data.rd.child, ret))
	{
		node->u_data.rd.exec_infos.ret = ret;
		return (true);
	}
	else if (node && node->type == ND_SUBSHELL)
	{
		node->u_data.subsh.exec_infos.ret = ret;
		return (true);
	}
	else if (node && node->type == ND_CMD)
	{
		node->u_data.cmd.exec_infos.ret = ret;
		return (true);
	}
	return (false);
}

void	rd_exit_clean(uint8_t code, t_sh_ctx *ctx, t_ast *node,
																int32_t fds[2])
{
	if (is_builtin(node) && _get_node_pid(node) == 0)
	{
		ft_close_pipe(fds);
		_set_node_ret(node, code);
	}
	else
	{
		context_free(ctx);
		ft_close_all(fds);
		exit(code);
	}
}
