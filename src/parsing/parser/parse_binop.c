/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_binop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:30:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/13 03:49:39 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/ast.h"

static inline void	*_set_errtok_quit(t_token **errtok, t_token *cur, t_ast *to_free)
{
	if (!*errtok)
		*errtok = cur;
	if (to_free)
		ast_free(to_free);
	return (NULL);
}

// right_associative
t_ast	*pipe_parser(t_token **cur, t_token **errtok)
{
	t_ast	*node;
	t_ast	*left;

	left = redir_parser(cur, errtok);
	if (!left)
		return (_set_errtok_quit(errtok, *cur, NULL));
	if (!*cur || (*cur)->type != TK_PIPE)
		return (left);
	next(cur);
	node = ft_calloc(1, sizeof (t_ast));
	if (!node)
		return (ast_free(left), NULL);
	node->type = ND_PIPE;
	node->u_data.op.left = left;
	node->u_data.op.right = pipe_parser(cur, errtok);
	if (!node->u_data.op.right)
		return (_set_errtok_quit(errtok, *cur, node));
	return (node);
}

static inline bool	_is_logicalop(t_tk_type type)
{
	return (type == TK_AND || type == TK_OR);
}

// left associative
t_ast	*logical_parser(t_token **cur, t_token **errtok)
{
	t_ast	*node;
	t_ast	*left;

	left = pipe_parser(cur, errtok);
	if (!left)
		return (_set_errtok_quit(errtok, *cur, NULL));
	while (*cur && _is_logicalop((*cur)->type))
	{
		node = ft_calloc(1, sizeof (t_ast));
		if (!node)
			return (ast_free(left), NULL);
		if ((*cur)->type == TK_AND)
			node->type = ND_AND;
		else
			node->type = ND_OR;
		node->u_data.op.left = left;
		next(cur);
		node->u_data.op.right = pipe_parser(cur, errtok);
		if (!node->u_data.op.right)
			return (_set_errtok_quit(errtok, *cur, node));
		left = node;
	}
	return (left);
}
