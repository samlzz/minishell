/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:23:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 15:58:22 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>
#include <unistd.h>
//TODO: tmp (for DEBUG)
#include "minishell.h"

/**
 * @brief Free all memory used by an AST node and its children.
 * 
 * Recursively frees the entire AST subtree. For heredoc nodes,
 * the temporary file is also unlinked.
 * 
 * @param node A pointer to the root of the AST subtree to free.
 */
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
		if (!KEEP_HD_FILES && node->u_data.rd.redir_type == RD_HEREDOC 
		&& node->u_data.rd.filename)
			unlink(node->u_data.rd.filename);
		free(node->u_data.rd.filename);
		ast_free(node->u_data.rd.child);
	}
	else if (node->type == ND_PIPE || \
		node->type == ND_AND || node->type == ND_OR)
	{
		ast_free(node->u_data.op.left);
		ast_free(node->u_data.op.right);
	}
	else if (node->type == ND_SUBSHELL)
	{
		ast_free(node->u_data.s_subsh.child);
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
