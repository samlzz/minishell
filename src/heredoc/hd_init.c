/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:08:12 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 10:24:05 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"

#ifdef MINISHELL_BONUS

uint8_t	hd_rec_init(t_ast *node)
{
	uint8_t	ec;

	if (!node)
		return (0);
	else if (node->type == ND_REDIR && node->u_data.rd.redir_type == RD_HEREDOC)
	{
		ec = hd_process(&node->u_data.rd);
		if (ec)
			return (ec);
		return (hd_rec_init(node->u_data.rd.child));
	}
	else if (node->type == ND_REDIR)
		return (hd_rec_init(node->u_data.rd.child));
	else if (node->type == ND_PIPE \
		|| node->type == ND_AND || node->type == ND_OR)
	{
		if (hd_rec_init(node->u_data.op.left))
			return (1);
		return (hd_rec_init(node->u_data.op.right));
	}
	else if (node->type == ND_SUBSHELL)
		return (hd_rec_init(node->u_data.subsh.child));
	return (0);
}

#else

uint8_t	hd_rec_init(t_ast *node)
{
	uint8_t	ec;

	if (!node)
		return (0);
	else if (node->type == ND_REDIR && node->u_data.rd.redir_type == RD_HEREDOC)
	{
		ec = hd_process(&node->u_data.rd);
		if (ec)
			return (ec);
		return (hd_rec_init(node->u_data.rd.child));
	}
	else if (node->type == ND_REDIR)
		return (hd_rec_init(node->u_data.rd.child));
	else if (node->type == ND_PIPE)
	{
		if (hd_rec_init(node->u_data.op.left))
			return (1);
		return (hd_rec_init(node->u_data.op.right));
	}
	return (0);
}

#endif
