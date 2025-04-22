/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:23:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/22 15:26:01 by sliziard         ###   ########.fr       */
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
	else if (node->type == ND_SUBSHELL)
	{
		ast_free(node->u_data.s_subsh.child);
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
 * @param env      A pointer to the environment hashmap, used for expansions.
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
 * @see [tokenise, binop_parser]
 */
t_ast	*new_ast(t_token *tokens, t_token **errtok, int16_t *errcode)
{
	t_ast	*ast;
	t_token	*cursor;

	cursor = tokens;
	ast = binop_parser(&cursor, ND_OR, errtok);
	if (!ast)
		return (NULL);
	if (cursor && cursor->type != TK_EOF)
	{
		*errcode = PARSE_ERR_EOF;
		if (!*errtok)
			*errtok = cursor;
		return (ast_free(ast), NULL);
	}
	return (ast);
}

t_ast	*parse_input(t_hmap *env, const char *input, \
	int16_t *err_code, t_token **errtok)
{
	t_token	*tokens;
	t_token	*expanded;
	t_ast	*ast;

	*err_code = PARSE_OK;
	*errtok = NULL;
	tokens = tokenise(input, err_code);
	if (!tokens)
		return (NULL);
	expanded = expand_token_list(env, tokens, errtok);
	*errtok = token_pop(&tokens, *errtok);
	token_clear(tokens);
	if (!expanded)
		return (NULL);
	ast = new_ast(expanded, errtok, err_code);
	*errtok = token_pop(&expanded, *errtok);
	token_clear(expanded);
	return (ast);
}
