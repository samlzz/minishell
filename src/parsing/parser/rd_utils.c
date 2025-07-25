/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 10:29:34 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 10:29:37 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/ast.h"
#include "lexer/token.h"

bool	is_redirection(t_tk_type type)
{
	return (type == TK_REDIR_IN || type == TK_REDIR_OUT
		|| type == TK_REDIR_APPEND || type == TK_HEREDOC);
}

t_redir_type	get_rd_type(t_tk_type tk)
{
	if (tk == TK_REDIR_IN)
		return (RD_IN);
	if (tk == TK_REDIR_OUT)
		return (RD_OUT);
	if (tk == TK_REDIR_APPEND)
		return (RD_APPEND);
	if (tk == TK_HEREDOC)
		return (RD_HEREDOC);
	return (-1);
}
