/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:23:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/03 17:53:46 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

void	ast_free(t_ast	*node)
{
	if (!node)
		return ;
	if (node->type == ND_CMD)
	{
		ft_splitfree(node->u_data.s_cmd.argv, 0);
	}
	else if (node->type == ND_REDIR)
	{
		free(node->u_data.s_redir.filename);
		ast_free(node->u_data.s_redir.child);
	}
	else if (node->type == ND_PIPE || \
		node->type == ND_AND || node->type == ND_OR)
	{
		ast_free(node->u_data.s_binop.left);
		ast_free(node->u_data.s_binop.right);
	}
	free(node);
}

/**
 * @brief Builds an abstract syntax tree (AST) from an input command string.
 *
 * This function first tokenizes the input using `tokenise`, then parses the token
 * list with `binop_parser`, using a top-down recursive descent parser to build
 * the AST corresponding to the shell command structure.
 *
 * @param input     The input string to parse (user command line).
 * @param errcode   A pointer to an int16_t that will hold the error code
 * 	(0 = success, -1 = internal error, -2 | -3 = unclosed quote).
 * @param errtok    A pointer to the token that caused the parse error, if any (NULL otherwise).
 *
 * @return A pointer to the root of the AST on success, or NULL on error.
 *
 * @note If tokenisation fails (e.g. unclosed quotes or memory allocation failure),
 *       the function returns NULL, sets `*errtok = NULL`, and fills `*errcode` with
 *       the appropriate error code.
 *
 * @note If parsing fails, `*errtok` is set to the token that caused the error,
 *       and `*errcode` describes the nature of the error. If the error comes from program
 *       itself (e.g., allocation failed) it sets `*errtok = NULL` and `*errcode = -1`; 
 * 
 * @see [tokenise, binop_parser, cmd_parser]
 */
t_ast	*new_ast(const char *input, int16_t *errcode, t_token **errtok)
{
	t_ast	*ast;
	t_token	*tokens;
	t_token	*cursor;

	*errcode = PARSE_OK;
	*errtok = NULL;
	tokens = tokenise(input, errcode);
	if (!tokens)
		return (NULL);
	cursor = tokens;
	ast = binop_parser(&cursor, ND_OR, errtok);
	if (!ast)
	{
		*errtok = token_pop(&tokens, *errtok);
		return (token_clear(tokens), NULL);
	}
	if (cursor && cursor->type != TK_EOF)
	{
		if (!*errtok)
			*errtok = token_pop(&tokens, cursor);
		*errcode = PARSE_ERR_EOF;
		token_clear(tokens);
		return (ast_free(ast), NULL);
	}
	return (token_clear(tokens), ast);
}
