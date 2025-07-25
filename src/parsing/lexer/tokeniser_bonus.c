/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:17:47 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 10:33:40 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

#ifdef MINISHELL_BONUS

int32_t	_fill_token(const char *input, t_token *curr)
{
	if (!*input)
		return (curr->type = TK_EOF, 0);
	else if (*input == '|')
		curr->type = TK_PIPE;
	else if (input[0] == '>')
		curr->type = (input[1] == '>') * TK_REDIR_APPEND
			+ !(input[1] == '>') * TK_REDIR_OUT;
	else if (input[0] == '<')
		curr->type = (input[1] == '<') * TK_HEREDOC
			+ !(input[1] == '<') * TK_REDIR_IN;
	else
		return (_handle_word(input, curr));
	return (1 + (curr->type == TK_REDIR_APPEND || curr->type == TK_HEREDOC));
}

#endif