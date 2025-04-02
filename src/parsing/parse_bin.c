/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:30:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/02 16:22:29 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static inline t_node_type	ttk_to_tnode(t_tk_type tk)
{
	if (tk == TK_PIPE)
		return ND_PIPE;
	if (tk == TK_AND)
		return ND_AND;
	if (tk == TK_OR)
		return ND_OR;
	return -1;
}

t_ast	*binop_parser(t_token **cur, t_node_type bin_op, int16_t *err)
{
	t_ast	*node;
	t_ast	*left;

	if (bin_op < ND_PIPE)
		return (cmd_parser(cur, err));
	if (bin_op > ND_OR)
		return (*err = PARSE_ERR, NULL);
	left = binop_parser(cur, bin_op - 1, err);
	if (!left)
		return (NULL);
	while (*cur && ttk_to_tnode((*cur)->type) == bin_op)
	{
		node = ft_calloc(1, sizeof (t_ast));
		if (!node)
			return (ast_free(left), *err = PARSE_ERR, NULL);
		node->type = bin_op;
		node->u_data.s_binop.left = left;
		*cur = (*cur)->next;
		node->u_data.s_binop.right = binop_parser(cur, bin_op - 1, err);
		if (!node->u_data.s_binop.right)
			return (ast_free(node), NULL);
		left = node;
	}
	return (left);
}
