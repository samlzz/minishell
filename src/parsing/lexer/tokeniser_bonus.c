/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:17:47 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 11:15:22 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include "error/error.h"

#ifdef MINISHELL_BONUS

int32_t	_fill_token(const char *input, t_token *curr)
{
	if (!*input)
		return (curr->type = TK_EOF, 0);
	else if (*input == '(')
		curr->type = TK_LPAREN;
	else if (*input == ')')
		curr->type = TK_RPAREN;
	else if (*input == '|' && input[1] == '|')
		curr->type = TK_OR;
	else if (*input == '|')
		curr->type = TK_PIPE;
	else if (*input == '&' && input[1] == '&')
		curr->type = TK_AND;
	else if (*input == '&')
		return (PARSE_ERR_SOLO_AND);
	else if (input[0] == '>')
		curr->type = (input[1] == '>') * TK_REDIR_APPEND
			+ !(input[1] == '>') * TK_REDIR_OUT;
	else if (input[0] == '<')
		curr->type = (input[1] == '<') * TK_HEREDOC
			+ !(input[1] == '<') * TK_REDIR_IN;
	else
		return (_handle_word(input, curr));
	return (1 + (curr->type == TK_OR || curr->type == TK_AND \
		|| curr->type == TK_REDIR_APPEND || curr->type == TK_HEREDOC));
}

#endif