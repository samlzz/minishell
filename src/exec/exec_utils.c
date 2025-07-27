/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 11:17:06 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/27 08:24:16 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <unistd.h>

#include "ast/ast.h"
#include "env/env.h"
#include "exec/exec.h"
#include "libft.h"

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

void	exec_wait_set_ret(t_ast *node, pid_t pid, uint8_t ret, int32_t status)
{
	t_expr	*infos;

	infos = NULL;
	if (node && node->type == ND_PIPE)
	{
		exec_wait_set_ret(node->u_data.op.left, pid, ret, status);
		exec_wait_set_ret(node->u_data.op.right, pid, ret, status);
	}
	else if (node && node->type == ND_REDIR
		&& node->u_data.rd.exec_infos.pid == pid)
		infos = &node->u_data.rd.exec_infos;
	else if (node && node->type == ND_REDIR)
		exec_wait_set_ret(node->u_data.rd.child, pid, ret, status);
	else if (node && node->type == ND_SUBSHELL
		&& node->u_data.subsh.exec_infos.pid == pid)
		infos = &node->u_data.subsh.exec_infos;
	else if (node && node->type == ND_CMD
		&& node->u_data.cmd.exec_infos.pid == pid)
		infos = &node->u_data.cmd.exec_infos;
	if (infos)
	{
		infos->ret = ret;
		infos->status = status;
	}
}

static inline void	_init_sig_empty_messages(char **msg)
{
	size_t	i;

	i = -1;
	while (++i < 256)
		msg[i] = "";
}

static inline void	_init_sig_messages(char **msg)
{
	_init_sig_empty_messages(msg);
	msg[1] = "Hangup";
	msg[3] = "Quit";
	msg[4] = "Illegal instruction";
	msg[5] = "Trace/breakpoint trap";
	msg[6] = "Aborted";
	msg[7] = "Bus error";
	msg[8] = "Floating point exception";
	msg[9] = "Killed";
	msg[10] = "User defined signal 1";
	msg[11] = "Segmentation fault";
	msg[12] = "User defined signal 2";
	msg[14] = "Alarm clock";
	msg[15] = "Terminated";
	msg[16] = "Stack fault";
	msg[24] = "CPU time limit exceeded";
	msg[25] = "File size limit exceeded";
	msg[26] = "Virtual timer expired";
	msg[27] = "Profiling timer expired";
	msg[29] = "I/O possible";
	msg[30] = "Power failure";
	msg[31] = "Bad system call";
}

void	print_sig_message(uint8_t sig, bool coredump)
{
	char	*msg[256];

	_init_sig_messages(msg);
	ft_putstr_fd(msg[sig], STDERR_FILENO);
	if (coredump)
		ft_putstr_fd(" (core dumped)", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}
