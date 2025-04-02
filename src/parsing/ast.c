/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:23:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/02 12:19:33 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

void	ast_free(t_ast	*node)
{
	if (!node)
		return ;
	if (node->type == ND_CMD)
		ft_splitfree(node->u_data.s_cmd.argv, 0);
	else if (node->type == ND_REDIR)
	{
		free(node->u_data.s_redir.filename);
		ast_free(node->u_data.s_redir.child);
	}
	else if (node->type == ND_PIPE)
	{
		ast_free(node->u_data.s_binop.left);
		ast_free(node->u_data.s_binop.right);
	}
	free(node);
}

t_ast	*new_ast(t_token *tokens)
{
	t_ast	*ast;

	if (!tokens)
		return (NULL);
	ast = binop_parser(&tokens);
	if (!ast)
		return (NULL);
	if (tokens && tokens->type != TK_EOF)
	{
		ast_free(ast);
		return (NULL);
	}
	return (ast);
}
