/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:23:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 11:22:50 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "error/error.h"
#include "parser/parser.h"

#ifndef MINISHELL_BONUS

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
				break ;
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
 * Recursively frees the entire AST subtree.
 * 
 * @param node A pointer to the root of the AST subtree to free.
 */
void	ast_free(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == ND_CMD)
		_words_free(node->u_data.cmd.args, node->u_data.cmd.is_expanded, true);
	else if (node->type == ND_REDIR)
	{
		_words_free(&node->u_data.rd.filename, node->u_data.rd.is_expanded,
			false);
		if (node->u_data.rd.redir_type == RD_HEREDOC && node->u_data.rd.fd > 0)
			close(node->u_data.rd.fd);
		ast_free(node->u_data.rd.child);
	}
	else if (node->type == ND_PIPE)
	{
		ast_free(node->u_data.op.left);
		ast_free(node->u_data.op.right);
	}
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
	ast = pipe_parser(&cursor, errtok);
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

#endif

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
		return (err_print(errcode, errtok, true), NULL);
	res = new_ast(tk_lst, &errtok, &errcode);
	errtok = token_pop(&tk_lst, errtok);
	token_clear(tk_lst);
	if (!res)
		return (err_print(errcode, errtok, false), NULL);
	return (res);
}
