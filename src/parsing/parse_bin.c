/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:30:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/01 16:28:51 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static bool	_is_binop(t_tk_type type)
{
	return (type == TK_AND || type == TK_OR || type == TK_PIPE);
}

static inline t_node_type	_get_bo_type(t_tk_type tk_type)
{
	t_node_type	type;

	type = -1;
	if (tk_type == TK_AND)
		type = ND_AND;
	else if (tk_type == TK_OR)
		type = ND_OR;
	else if (tk_type == TK_PIPE)
		type = ND_PIPE;
	return (type);
}

t_ast	*binop_parser(t_token **cur)
{
	t_ast	*pipe;
	t_ast	*left;

	left = cmd_parser(cur);
	if (!left)
		return (NULL);
	while (*cur && _is_binop((*cur)->type))
	{
		pipe = ft_calloc(1, sizeof (t_ast));
		if (!pipe)
			return (ast_free(left), NULL);
		pipe->type = _get_bo_type((*cur)->type);
		pipe->u_data.s_binop.left = left;
		*cur = (*cur)->next;
		pipe->u_data.s_binop.right = cmd_parser(cur);
		if (!pipe->u_data.s_binop.right)
			return (ast_free(pipe), NULL);
		left = pipe;
	}
	return (left);
}