/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:23:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/02 13:48:53 by sliziard         ###   ########.fr       */
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

/**
 * @param err(int16_t):
 * 
 * -1 = internal error
 * -2 | -3 = single or double cote not closed
 * -4 = char not handled
 * -5 = no word when expected
 * -6 = no bin operation when expected
 */
int16_t	new_ast(const char *input, t_ast **new)
{
	t_token	*tokens;
	int16_t	exit_code;

	tokens = tokenise(input, &exit_code);
	if (!tokens)
		return (exit_code);
	*new = binop_parser(&tokens, &exit_code);
	if (!*new)
	{
		token_clear(tokens);
		return (exit_code);
	}
	if (tokens && tokens->type != TK_EOF)
	{
		token_clear(tokens);
		ast_free(*new);
		return (PARSE_ERR_EOF);
	}
	return (0);
}
