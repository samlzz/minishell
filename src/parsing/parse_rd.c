/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:36:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/07 11:11:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static inline t_redir_type	_get_rd_type(t_tk_type tk)
{
	if (tk == TK_REDIR_IN)
		return RD_IN;
	if (tk == TK_REDIR_OUT)
		return RD_OUT;
	if (tk == TK_REDIR_APPEND)
		return RD_APPEND;
	if (tk == TK_HEREDOC)
		return RD_HEREDOC;
	return -1;
}

static inline t_ast	*_parse_redirection(t_token **cur, t_ast *tree, t_token **errtok)
{
	t_ast			*node;
	t_redir_type	type;
	t_token			*filename;

	if (!*cur || !_is_redirection((*cur)->type))
		return (*errtok = *cur, NULL);
	type = _get_rd_type((*cur)->type);
	*cur = (*cur)->next;
	if (!*cur || (*cur)->type != TK_WORD)
		return (*errtok = *cur, NULL);
	filename = *cur;
	*cur = (*cur)->next;
	node = ft_calloc(1, sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = ND_REDIR;
	node->u_data.s_redir.redir_type = type;
	node->u_data.s_redir.filename = ft_strdup(filename->value);
	node->u_data.s_redir.child = tree;
	return (node);
}

bool	handle_redirection(t_token **cur, t_ast **tree, \
	t_token **errtok)
{
	t_ast	*redir;
	
	redir = _parse_redirection(cur, *tree, errtok);
	if (!redir)
		return (false);
	*tree = redir;
	return (true);
}
