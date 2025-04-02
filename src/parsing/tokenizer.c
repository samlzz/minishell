/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:17:47 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/01 21:00:24 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

/**
 * @return
 * exit_code(int32_t):
 * 	- `-1` = internal error
 *  - `-2` = single cote not closed
 *  - `-3` = double cote not closed
 *  - `-4` = char not handled
 */
static inline int32_t	_handle_quoted_word(char *input, t_token *curr)
{
	int32_t	len;

	len = 1;
	while (input[len] && input[len] != *input)
		len++;
	if (!input[len])
		return ((*input == '"') * -3 + !(*input == '"') * -2);
	curr->value = ft_substr(input, 1, len - 1);
	if (!curr->value)
		return (-1);
	if (*input == '"')
		curr->quote = QUOTE_DOUBLE;
	else
		curr->quote = QUOTE_SINGLE;
	return (len + 1);
}

static inline int32_t	_handle_word(char *input, t_token *curr)
{
	int32_t	len;

	curr->type = TK_WORD;
	if (*input == '"' || *input == '\'')
		return (_handle_quoted_word(input, curr));
	else if (ft_isalnum(*input) || ft_strchr("./-_$", *input))
	{
		len = 0;
		while (input[len] && !ft_strchr(" <>|&'\"\t\n", input[len]))
			len++;
		curr->value = ft_substr(input, 0, len);
		if (!curr->value)
			return (-1);
		curr->quote = QUOTE_NONE;
		return (len);
	}
	return (-4);
}

static inline int32_t	_fill_token(char *input, t_token *curr)
{
	if (!*input)
	{
		curr->type = TK_EOF;
		return (0);
	}
	else if (*input == '|')
	{
		if (input[1] == '|')
			curr->type = TK_OR;
		else
			curr->type = TK_PIPE;
	}
	else if (*input == '&' && input[1] == '&')
		curr->type = TK_AND;
	else if (input[0] == '>')
		curr->type = (input[1] == '>') * TK_REDIR_APPEND \
			+ !(input[1] == '>') * TK_REDIR_OUT;
	else if (input[0] == '<')
		curr->type = (input[1] == '<') * TK_HEREDOC \
			+ !(input[1] == '<')  *TK_REDIR_IN;
	else
		return (_handle_word(input, curr));
	return (1 + (curr->type == TK_OR || curr->type == TK_AND || \
		curr->type == TK_REDIR_APPEND || curr->type == TK_HEREDOC));
}

t_token	*tokenise(char *input)
{
	size_t	i;
	int32_t	offset;
	t_token	*tokens;
	t_token	*curr;

	i = 0;
	tokens = NULL;
	while (input[i])
	{
		curr = ft_calloc(1, sizeof (t_token));
		if (!curr)
			return (token_clear(tokens), NULL);
		while (input[i] == ' ')
			i++;
		offset = _fill_token(input + i, curr);
		if (offset < 0)
			return (token_clear(tokens), free(curr), NULL);
		i += offset;
		token_addback(&tokens, curr);
	}
	return (tokens);
}
