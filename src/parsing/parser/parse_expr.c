/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:24:10 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/06 17:26:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/ast.h"
#include <stdlib.h>

/**
 * @brief Join two argv arrays into a single null-terminated array.
 * 
 * Does not duplicate strings, just merges two pointer arrays.
 * 
 * @param og First argv array.
 * @param new Second argv array.
 * @return char** New merged argv array (allocated).
 */
char	**join_argv(char **og, char **new)
{
	int32_t	second;
	int32_t	first;
	int32_t	i;
	char	**dest;

	first = 0;
	while (og[first])
		first++;
	second = 0;
	while (new[second])
		second++;
	dest = malloc((first + second + 1) * sizeof (char *));
	if (!dest)
		return (NULL);
	i = -1;
	while (++i < first)
		dest[i] = og[i];
	i = -1;
	while (++i < second)
		dest[first + i] = new[i];
	dest[first + i] = NULL;
	return (dest);
}

/**
 * @brief Collect a sequence of TK_WORD tokens into an argv array.
 * 
 * Steals ownership of token values (sets them to NULL).
 * 
 * @param cur Current token cursor.
 * @param errtok Output error token if parsing fails.
 * @return char** Array of arguments or NULL on error.
 */
char	**collect_argv(t_token **cur, t_token **errtok)
{
	char	**argv;
	size_t	size;
	size_t	i;
	t_token	*cursor;

	size = 0;
	cursor = *cur;
	while (cursor && cursor->type == TK_WORD)
		(size++, (cursor = cursor->next));
	if (!size)
		return ((*errtok = cursor), NULL);
	argv = malloc(sizeof (char *) * (size + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (i < size)
	{
		argv[i++] = (*cur)->value;
		(*cur)->value = NULL;
		next(cur);
	}
	argv[i] = NULL;
	return (argv);
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
		return (NULL);
	node->type = ND_CMD;
	node->u_data.s_cmd.argv = collect_argv(cur, errtok);
	if (!node->u_data.s_cmd.argv)
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
		subexpr = new_binop_parser(cur, LVL_LOGICAL, errtok);
		if (!*cur || !subexpr || (*cur)->type != TK_RPAREN)
			return (ast_free(subexpr), NULL);
		next(cur);
		subshell = ft_calloc(1, sizeof(t_ast));
		if (!subshell)
			return (ast_free(subexpr), NULL);
		subshell->type = ND_SUBSHELL;
		subshell->u_data.s_subsh.child = subexpr;
		return (subshell);
	}
	return (cmd_parser(cur, errtok));
}
