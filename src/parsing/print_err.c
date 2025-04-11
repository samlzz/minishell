/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:47:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/11 17:56:56 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static inline const char	*token_type_str(t_tk_type type)
{
	const char	*stringify[TK_EOF + 1];

	stringify[TK_WORD] = "WORD";
	stringify[TK_PIPE] = "|";
	stringify[TK_AND] = "&&";
	stringify[TK_OR] = "||";
	stringify[TK_LPAREN] = "(";
	stringify[TK_RPAREN] = ")";
	stringify[TK_REDIR_IN] = "<";
	stringify[TK_REDIR_OUT] = ">";
	stringify[TK_REDIR_APPEND] = ">>";
	stringify[TK_HEREDOC] = "<<";
	stringify[TK_EOF] = "EOF";
	return (stringify[type]);
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
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	ft_putendl_fd(tk, 2);
	token_clear(errtok);
}
