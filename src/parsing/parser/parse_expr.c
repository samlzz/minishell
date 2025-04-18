/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:24:10 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/17 23:45:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

static char	**_collect_argv(t_hmap *env, t_token **cur, t_token **errtok)
{
	char		**argv;
	t_argword	*args;
	t_argword	*new;
	t_argword	*wild;

	args = NULL;
	while (*cur && (*cur)->type == TK_WORD)
	{
		new = expand_and_join_words(env, cur, 0);
		if (!new)
			return (argword_clear(args), NULL);
		if (new->wild_offsets.len)
		{
			wild = expand_wildcards(new);
			if (wild)
			{
				argword_clear(new);
				new = wild;
			}
		}
		argword_add_back(&args, new);
	}
	if (!args)
		*errtok = *cur;
	argv = argwords_to_argv(args);
	argword_clear(args);
	return (argv);
}

/**
 * @brief Parses a simple command node (`ND_CMD`) from the token stream.
 *
 * This function collects a sequence of `TK_WORD` tokens and expands them to
 * build the argv array of a ND_CMD AST node. It does not handle redirections
 * or subshells
 * — those must be wrapped around the result by a higher-level parser.
 *
 * @param cur		A pointer to the current token pointer in the stream.
 * @param errtok	A pointer to the token where an error occurred, if any.
 *
 * @return A pointer to a `ND_CMD` AST node, or `NULL` if parsing failed.
 */
t_ast	*cmd_parser(t_hmap *env, t_token **cur, t_token **errtok)
{
	t_ast	*node;

	node = ft_calloc(1, sizeof (t_ast));
	if (!node)
		return (NULL);
	node->type = ND_CMD;
	node->u_data.s_cmd.argv = _collect_argv(env, cur, errtok);
	if (!node->u_data.s_cmd.argv)
		return (free(node), NULL);
	return (node);
}

/**
 * @brief Parses a primary expression from the token stream: either a simple
 *        command or a parenthesized subshell expression.
 *
 * This function is called when no binary operator has higher precedence,
 * and is responsible for handling:
 * - a command (sequence of TK_WORD / redirections), via `cmd_parser`,
 * - a parenthesized expression, parsed recursively with `binop_parser`,
 *   and wrapped in a ND_SUBSHELL node in the AST.
 *
 * @param cur		A pointer to the current token pointer in the stream.
 * @param errtok	A pointer to the token pointer where a parsing error 
 * occurred, if any.
 *
 * @return A pointer to the resulting AST node (command or subshell),
 *         or NULL in case of error.
 *
 * @note If a subshell is detected (starting with `TK_LPAREN`), the function 
 *        ensures that it is properly closed by a `TK_RPAREN`. 
 *        If the closing parenthesis is missing, `*errtok` is set accordingly,
 *        and any allocated subtree is freed.
 */
t_ast	*primary_parser(t_hmap *env, t_token **cur, t_token **errtok)
{
	t_ast	*subexpr;
	t_ast	*subshell;

	if (*cur && (*cur)->type == TK_LPAREN)
	{
		*cur = (*cur)->next;
		subexpr = binop_parser(env, cur, ND_OR, errtok);
		if (!subexpr)
			return (NULL);
		if (!*cur || (*cur)->type != TK_RPAREN)
			return (*errtok = *cur, ast_free(subexpr), NULL);
		*cur = (*cur)->next;
		subshell = ft_calloc(1, sizeof(t_ast));
		if (!subshell)
			return (ast_free(subexpr), NULL);
		subshell->type = ND_SUBSHELL;
		subshell->u_data.s_subsh.child = subexpr;
		return (subshell);
	}
	return (cmd_parser(env, cur, errtok));
}
