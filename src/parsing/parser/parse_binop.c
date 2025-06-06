/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_binop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:30:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/06 17:24:57 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/ast.h"

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
 * @brief Parse binary operations (`||`, `&&`, `|`) with correct precedence.
 * 
 * Builds a binary expression tree from the token stream. Recursively
 * calls itself with decreasing precedence levels until it call `redir_parser`
 * and builds the AST.
 * 
 * @param cur Pointer to the current token.
 * @param bin_op The binary operation type to parse at this level.
 * @param errtok Output pointer for the error token if parsing fails.
 * @return t_ast* AST subtree for the binary operation or NULL on error.
 * 
 * @see redir_parser
 */
t_ast	*binop_parser(t_token **cur, t_node_type bin_op, \
	t_token **errtok)
{
	t_ast	*node;
	t_ast	*left;

	if (bin_op < ND_PIPE)
		return (redir_parser(cur, errtok));
	if (bin_op > ND_OR)
		return (NULL);
	left = binop_parser(cur, bin_op - 1, errtok);
	if (!left)
		return ((void)(!*errtok && (*errtok = *cur)), NULL);
	while (*cur && ttk_to_tnode((*cur)->type) == bin_op)
	{
		node = ft_calloc(1, sizeof (t_ast));
		if (!node)
			return (ast_free(left), NULL);
		node->type = bin_op;
		node->u_data.op.left = left;
		next(cur);
		node->u_data.op.right = binop_parser(cur, bin_op - 1, errtok);
		if (!node->u_data.op.right)
			return (ast_free(node), *errtok = *cur, NULL);
		left = node;
	}
	return (left);
}

static inline bool	_belongs_to_group(t_tk_type type, t_parse_lvl prior)
{
    if (prior == LVL_LOGICAL)
        return (type == TK_OR || type == TK_AND);
    if (prior == LVL_PIPE)
        return (type == TK_PIPE);
    return (false);
}

t_ast	*new_binop_parser(t_token **cur, t_parse_lvl prior, t_token **errtok)
{
	t_ast	*node;
	t_ast	*left;

	if (prior < LVL_LOGICAL)
		return (NULL);
	if (prior == LVL_EXPR)
		return (redir_parser(cur, errtok));
	left = new_binop_parser(cur, prior + 1, errtok);
	if (!left)
		return ((void)(!*errtok && (*errtok = *cur)), NULL);
	while (*cur && _belongs_to_group((*cur)->type, prior))
	{
		node = ft_calloc(1, sizeof (t_ast));
		if (!node)
			return (ast_free(left), NULL);
		node->type = ttk_to_tnode((*cur)->type);
		node->u_data.op.left = left;
		next(cur);
		node->u_data.op.right = new_binop_parser(cur, prior + 1, errtok);
		if (!node->u_data.op.right)
			return (ast_free(node), *errtok = *cur, NULL);
		left = node;
	}
	return (left);
}

/*
t_ast	*logicalop_parser(t_token **cur, t_token **errtok)
{
	t_ast	*node;
	t_ast	*left;

	left = logicalop_parser(cur, errtok) || pipe_parser(cur, errtok);
	if (!left)
		return (NULL);
	while (*cur && ((*cur)->type == TK_AND || (*cur)->type == TK_OR))
	{
		node = ft_calloc(1, sizeof (t_ast));
		if (!node)
			return (ast_free(left), NULL);
		node->type = ttk_to_tnode((*cur)->type);
		node->u_data.op.left = left;
		next(cur);
		node->u_data.op.right = logicalop_parser(cur, errtok) || pipe_parser(cur, errtok);
		if (!node->u_data.op.right)
			return (NULL);
	}
	
}
*/