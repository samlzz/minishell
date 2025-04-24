/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:47:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/24 19:55:06 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

static inline const char	*token_type_str(t_token *errtok)
{
	const char	*stringify[TK_EOF + 1];

	if (!errtok \
		|| errtok->type < TK_WORD \
		|| errtok->type > TK_EOF)
		return ("unknow");
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
	return (stringify[errtok->type]);
}

static inline void	_print_ambiguous_redirect(const char *raw)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(raw, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
}

static inline void	_print_unclosed_quote(int16_t errcode)
{
	if (errcode == PARSE_ERR_SQUOTE)
		ft_putendl_fd("minishell: unexpected EOF while looking for matching `\''", 2);
	else if (errcode == PARSE_ERR_DQUOTE)
		ft_putendl_fd("minishell: unexpected EOF while looking for matching `\"'", 2);
}

static void	_print_syntax_error(t_token *tok)
{
	const char *tk;

	if (!tok)
		tk = "newline";
	else if (tok->unexpanded)
		return (_print_ambiguous_redirect(tok->unexpanded));
	else if (tok->value)
		tk = tok->value;
	else
		tk = token_type_str(tok);
	ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
	ft_putendl_fd(tk, 2);
}

void	print_err(int16_t errcode, t_token *errtok)
{
	if (errcode == PARSE_ERR)
		ft_putendl_fd("minishell: internal error occurs", 2);
	else if (errcode == PARSE_ERR_SQUOTE || errcode == PARSE_ERR_DQUOTE)
		_print_unclosed_quote(errcode);
	else
		_print_syntax_error(errtok);
	token_clear(errtok);
}
