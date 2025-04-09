/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:47:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/09 18:50:25 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdio.h>

const char	*token_type_str(t_tk_type type)
{
	if (type == TK_WORD) return "WORD";
	if (type == TK_PIPE) return "|";
	if (type == TK_AND) return "&&";
	if (type == TK_OR) return "||";
	if (type == TK_LPAREN) return "(";
	if (type == TK_RPAREN) return ")";
	if (type == TK_REDIR_IN) return "<";
	if (type == TK_REDIR_OUT) return ">";
	if (type == TK_REDIR_APPEND) return ">>";
	if (type == TK_HEREDOC) return "<<";
	if (type == TK_EOF) return "EOF";
	return "newline";
}

void	print_err(int16_t errcode, t_token *errtok)
{
	const char	*tk;

	if (errcode == PARSE_ERR)
		return (ft_putendl_fd("minishell: internal error occurs", 2));
	else if (errcode == PARSE_ERR_SQUOTE)
		return (ft_putendl_fd("minishell: unexpected EOF while looking for matching `\''", 2));
	else if (errcode == PARSE_ERR_DQUOTE)
		return (ft_putendl_fd("minishell: unexpected EOF while looking for matching `\"'", 2));
	if (!errtok && errcode == PARSE_OK)
		tk = "newline";
	else
		tk = (const char *)errtok->value;
	if (!tk)
		tk = token_type_str(errtok->type);
	fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tk);
	fprintf(stderr, "[%d]\n", errcode);
	token_clear(errtok);
}
