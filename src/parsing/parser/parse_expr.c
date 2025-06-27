/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:24:10 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/27 20:12:07 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/ast.h"
#include <stdlib.h>

t_words	*collect_args(t_token **cur, t_token **errtok)
{
	t_words	*args;
	t_token	*new;

	args = ft_calloc(1, sizeof (t_words));
	if (!args)
		return (parse_err("minishell: malloc", NULL));
	while (*cur && (*cur)->type == TK_WORD)
	{
		new = token_dup(*cur);
		if (!new)
		{
			parse_err("minishell: token_dup", NULL);
			token_clear(args->tk);
			return (free(args), NULL);
		}
		token_addback(&args->tk, new);
		next(cur);
	}
	if (!args->tk)
	{
		*errtok = *cur;
		return (free(args), NULL);
	}
	return (args);
}

/**
 * @brief Parse a simple command into a `ND_CMD` node.
 * 
 * Collects tokens of type TK_WORD and forms the argv array.
 * 
 * @param cur Pointer to the token cursor.
 * @param errtok Output error token if parsing fails.
 * @return t_ast* AST node or NULL on error.
 */
t_ast	*cmd_parser(t_token **cur, t_token **errtok)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof (t_ast));
	if (!node)
		return (parse_err("minishell: malloc", NULL));
	node->type = ND_CMD;
	node->u_data.cmd.args = collect_args(cur, errtok);
	if (!node->u_data.cmd.args)
		return (free(node), NULL);
	return (node);
}


/**
 * @brief Parse either a command or a parenthesized subshell expression.
 * 
 * A primary expression can be a ND_CMD or a ND_SUBSHELL node.
 * 
 * @param cur Pointer to the token cursor.
 * @param errtok Output error token if parsing fails.
 * @return A pointer to the resulting AST node,or NULL in case of error.
 * 
 * @note If a subshell is detected (starting with `TK_LPAREN`), the function 
 *        ensures that it is properly closed by a `TK_RPAREN`.
 * @see cmd_parser, binop_parser
 */
t_ast	*primary_parser(t_token **cur, t_token **errtok)
{
	t_ast	*subexpr;
	t_ast	*subshell;

	if (*cur && (*cur)->type == TK_LPAREN)
	{
		next(cur);
		subexpr = logical_parser(cur, errtok);
		if (!*cur || !subexpr || (*cur)->type != TK_RPAREN)
			return (ast_free(subexpr), NULL);
		next(cur);
		subshell = ft_calloc(1, sizeof(t_ast));
		if (!subshell)
			return (parse_err("minishell: malloc", subexpr));
		subshell->type = ND_SUBSHELL;
		subshell->u_data.subsh.child = subexpr;
		return (subshell);
	}
	return (cmd_parser(cur, errtok));
}
