/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:23:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/26 10:19:47 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>
#include <unistd.h>
// TODO: for debug
#include "minishell.h"

static inline void	_words_free(t_words *val, bool expand, bool allocated)
{
	size_t	i;

	if (!val)
		return ;
	if (expand)
	{
		i = 0;
		while (val[i].expanded)
		{
			free(val[i++].expanded);
			if (!allocated)
				break;
		}
	}
	else
		token_clear(val->tk);
	if (allocated)
		free(val);
}

/**
 * @brief Free all memory used by an AST node and its children.
 * 
 * Recursively frees the entire AST subtree. For heredoc nodes,
 * the temporary file is also unlinked.
 * 
 * @param node A pointer to the root of the AST subtree to free.
 * @param expand A boolean to know wich value is used for t_words
 */
void	ast_free(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == ND_CMD)
		_words_free(node->u_data.cmd.args, node->u_data.cmd.is_expanded, true);
	else if (node->type == ND_REDIR)
	{
		_words_free(&node->u_data.rd.filename, node->u_data.rd.is_expanded, false);
		ast_free(node->u_data.rd.child);
	}
	else if (node->type == ND_PIPE || \
		node->type == ND_AND || node->type == ND_OR)
	{
		ast_free(node->u_data.op.left);
		ast_free(node->u_data.op.right);
	}
	else if (node->type == ND_SUBSHELL)
		ast_free(node->u_data.subsh.child);
	free(node);
}

/**
 * @brief Build an AST from a token list.
 * 
 * Top-level parser that handles full binary operator parsing.
 * Returns NULL if there's a parse error or unexpected tokens after the parse.
 * 
 * @param tokens The list of tokens to parse.
 * @param errtok Pointer to hold the token causing a parse error, if any.
 * @param errcode Pointer to hold the parse error code.
 * @param errcode Pointer to an int16_t to store the error code:
 *                    -  0 : success
 *                    - -1 : malloc failure
 *                    - -2 : unclosed single quote
 *                    - -3 : unclosed double quote
 *                    - -4 : some symbol is malformed
 *                    - -5 : unexpectedly, no EOF is found
 *                    - -6 : a lonely '&' was encountered
 * @return t_ast* The AST root node or NULL on error.
 */
t_ast	*new_ast(t_token *tokens, t_token **errtok, int16_t *errcode)
{
	t_ast	*ast;
	t_token	*cursor;

	cursor = tokens;
	ast = logical_parser(&cursor, errtok);
	if (!ast && !*errtok)
		*errcode = PARSE_ERR;
	if (!ast)
		return (NULL);
	if (cursor && cursor->type != TK_EOF)
	{
		*errcode = PARSE_ERR_EOF;
		if (!*errtok)
			*errtok = cursor;
		ast_free(ast);
		return (NULL);
	}
	return (ast);
}

#ifdef DEBUG_MODE

t_ast	*parse_ast(const char *input)
{
	t_ast	*res;
	int16_t	errcode;
	t_token	*errtok;
	t_token	*tk_lst;

	errcode = PARSE_OK;
	errtok = NULL;
	tk_lst = tokenise(input, &errcode);
	if (!tk_lst)
		return (err_print(errcode, errtok, false), NULL);
	if (PRINT_TOKENS)
		print_tokens(tk_lst);
	res = new_ast(tk_lst, &errtok, &errcode);
	errtok = token_pop(&tk_lst, errtok);
	token_clear(tk_lst);
	if (!res)
		return (err_print(errcode, errtok, true), NULL);
	return (res);
}

#else

t_ast	*parse_ast(const char *input)
{
	t_ast	*res;
	int16_t	errcode;
	t_token	*errtok;
	t_token	*tk_lst;

	errcode = PARSE_OK;
	errtok = NULL;
	tk_lst = tokenise(input, &errcode);
	if (!tk_lst)
		return (err_print(errcode, errtok, false), NULL);
	res = new_ast(tk_lst, &errtok, &errcode);
	errtok = token_pop(&tk_lst, errtok);
	token_clear(tk_lst);
	if (!res)
		return (err_print(errcode, errtok, true), NULL);
	return (res);
}
#endif