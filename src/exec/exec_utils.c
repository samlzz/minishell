/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:17:06 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/25 11:18:15 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "ast/ast.h"
#include "env/env.h"
#include "exec/exec.h"

size_t	exec_wait_get_count(t_ast *node, bool is_hd)
{
	if (!node)
		return (0);
	if (node->type == ND_PIPE || (is_hd && (node->type == ND_AND
				|| node->type == ND_OR)))
		return (exec_wait_get_count(node->u_data.op.left, is_hd)
			+ exec_wait_get_count(node->u_data.op.right, is_hd));
	else if (node->type == ND_SUBSHELL)
		return (node->u_data.subsh.exec_infos.pid > 0);
	else if (node->type == ND_REDIR)
	{
		if (node->u_data.rd.child && is_hd)
			return (exec_wait_get_count(node->u_data.rd.child, is_hd)
				+ node->u_data.rd.exec_infos.pid > 0);
		else if (node->u_data.rd.child)
			return (exec_wait_get_count(node->u_data.rd.child, is_hd));
		else
			return (node->u_data.rd.exec_infos.pid > 0);
	}
	else if (node->type == ND_CMD)
		return (node->u_data.cmd.exec_infos.pid > 0);
	return (0);
}

void	exec_wait_set_ret(t_ast *node, pid_t pid, uint8_t ret)
{
	if (node && node->type == ND_PIPE)
	{
		exec_wait_set_ret(node->u_data.op.left, pid, ret);
		exec_wait_set_ret(node->u_data.op.right, pid, ret);
	}
	else if (node && node->type == ND_REDIR
		&& node->u_data.rd.exec_infos.pid == pid)
		node->u_data.rd.exec_infos.ret = ret;
	else if (node && node->type == ND_REDIR)
		exec_wait_set_ret(node->u_data.rd.child, pid, ret);
	else if (node && node->type == ND_SUBSHELL
		&& node->u_data.subsh.exec_infos.pid == pid)
		node->u_data.subsh.exec_infos.ret = ret;
	else if (node && node->type == ND_CMD
		&& node->u_data.cmd.exec_infos.pid == pid)
		node->u_data.cmd.exec_infos.ret = ret;
}
