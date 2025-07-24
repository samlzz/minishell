/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 18:47:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/24 11:34:16 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <errno.h>

#include "error.h"
#include "libft.h"

void	err_print_expand(t_token *errtok)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(errtok->value, 2);
	ft_putstr_fd(": ambiguous redirect\n", 2);
}

#ifdef MINISHELL_BONUS

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
	stringify[TK_EOF] = "newline";
	return (stringify[errtok->type]);
}

#else

static inline const char	*token_type_str(t_token *errtok)
{
	const char	*stringify[TK_EOF + 1];

	if (!errtok \
		|| errtok->type < TK_WORD \
		|| errtok->type > TK_EOF)
		return ("unknow");
	stringify[TK_WORD] = "WORD";
	stringify[TK_PIPE] = "|";
	stringify[TK_REDIR_IN] = "<";
	stringify[TK_REDIR_OUT] = ">";
	stringify[TK_REDIR_APPEND] = ">>";
	stringify[TK_HEREDOC] = "<<";
	stringify[TK_EOF] = "newline";
	return (stringify[errtok->type]);
}

#endif

static void	_print_eof_error(t_token *errtok, char *raw)
{
	if ((!errtok && !raw) || (errtok && errtok->type == TK_EOF))
		return (ft_putendl_fd("minishell: unexpected end of file", 2));
	ft_putstr_fd("minishell: unexpected EOF while looking for matching `", 2);
	if (raw)
		ft_putstr_fd(raw, 2);
	else if (!errtok->value)
		ft_putstr_fd(token_type_str(errtok), 2);
	else
		ft_putstr_fd(errtok->value, 2);
	ft_putstr_fd("'\n", 2);
}

/**
 * @brief Print a syntax error message for an unexpected token.
 *
 * Chooses between various representations: unexpanded, value, 
 * or token type string.
 *
 * @param tok The erroneous token.
 * @param input_tk Optional hardcoded token string (e.g., "&").
 */
static void	_print_syntax_error(t_token *tok, char *input_tk)
{
	const char	*tk;

	if (input_tk)
		tk = input_tk;
	else if (!tok)
		tk = "newline";
	else if (tok->value)
		tk = tok->value;
	else
		tk = token_type_str(tok);
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(tk, 2);
	ft_putstr_fd("'\n", 2);
}

/**
 * @brief High-level error reporting function for parse errors.
 *
 * Dispatches messages based on error codes, then frees the error token.
 *
 * @param errcode The error code to report.
 * @param errtok The token where the error occurred (may be NULL).
 */
void	err_print(int16_t errcode, t_token *errtok, bool is_tokenize)
{
	if (errcode == PARSE_ERR)
	{
		if (!errno)
			return (token_clear(errtok));
		if (is_tokenize)
			perror("minishell: tokeniser: malloc");
		else
			perror("minishell: internal error");
	}
	else if (errcode == PARSE_ERR_EOF)
		_print_eof_error(errtok, NULL);
	else if (errcode == PARSE_ERR_SQUOTE)
		_print_eof_error(NULL, "\'");
	else if (errcode == PARSE_ERR_DQUOTE)
		_print_eof_error(NULL, "\"");
	else if (errcode == PARSE_ERR_MALFORMED)
		ft_putstr_fd("minishell: bad input\n", 2);
	else if (errcode == PARSE_ERR_SOLO_AND)
		_print_syntax_error(errtok, "&");
	else
		_print_syntax_error(errtok, NULL);
	token_clear(errtok);
}
