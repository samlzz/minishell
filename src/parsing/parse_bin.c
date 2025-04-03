/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:30:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/03 12:23:24 by sliziard         ###   ########.fr       */
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

/**
 * @brief Recursively parses binary operations (pipes, AND, OR) from the token stream.
 *
 * This function implements a precedence-based recursive descent parser for
 * binary operators (`||`, `&&`, `|`). It builds the corresponding AST nodes,
 * with each binary operator becoming a node with left and right children.
 *
 * @param cur      A pointer to the current token pointer in the stream.
 * @param bin_op   The binary operator node type to parse at this level (ND_PIPE, ND_AND, ND_OR).
 * @param errtok   A pointer to the token pointer where the parsing error occurred, if any.
 *
 * @return A pointer to the resulting AST subtree, or NULL if a parsing error occurred.
 *
 * @note In case of error, `*errtok` is set to the token at which the error was detected.
 *       Memory allocated during parsing is freed as needed to avoid leaks.
 */
t_ast	*binop_parser(t_token **cur, t_node_type bin_op, t_token **errtok)
{
	t_ast	*node;
	t_ast	*left;

	if (bin_op < ND_PIPE)
		return (cmd_parser(cur, errtok));
	if (bin_op > ND_OR || (*cur)->type == TK_EOF)
		return (NULL);
	left = binop_parser(cur, bin_op - 1, errtok);
	if (!left)
		return (NULL);
	while (*cur && ttk_to_tnode((*cur)->type) == bin_op)
	{
		node = ft_calloc(1, sizeof (t_ast));
		if (!node)
			return (ast_free(left), NULL);
		node->type = bin_op;
		node->u_data.s_binop.left = left;
		*cur = (*cur)->next;
		node->u_data.s_binop.right = binop_parser(cur, bin_op - 1, errtok);
		if (!node->u_data.s_binop.right)
			return (ast_free(node), NULL);
		left = node;
	}
	return (left);
}
