/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_tokeniser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:36:41 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 17:39:05 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include <stdlib.h>

static bool	_is_dollar(int c)
{
	return ((uint8_t)c == '$');
}

static bool	_is_token(int c)
{
	return (ft_isspace(c) || (uint8_t)c == '"');
}

static inline t_token	*_new_token(const char *input, size_t *start, \
	bool (*until_not) (int))
{
	size_t	len;
	t_token	*new;

	new = ft_calloc(1, sizeof (t_token));
	if (!new)
		return (NULL);
	len = *start;
	while (input[len] && !until_not(input[len]))
		len++;
	new->value = ft_substr(input, *start, len - *start);
	if (!new->value)
		return (free(new), NULL);
	*start = len;
	new->type = TK_WORD;
	if (ft_strchr(input, '$'))
		new->quote = QUOTE_DOUBLE;
	else
		new->quote = QUOTE_SINGLE;
	return (new);
}

/**
 * @brief Tokenizes a here-document string into WORD tokens.
 *
 * Uses `$` and `"` as separators to determine quote context (SINGLE or DOUBLE).
 * This is used to control whether expansions happen inside heredoc.
 *
 * @param input Raw string.
 * @return t_token* A list of tokens ending in TK_EOF, or NULL on failure.
 */
t_token	*hd_tokenise(const char *input)
{
	t_token	*lst;
	t_token	*cur;
	size_t	i;

	lst = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
			cur = _new_token(input, &i, &_is_token);
		else
			cur = _new_token(input, &i, &_is_dollar);
		if (!cur)
			return (token_clear(lst), NULL);
		token_addback(&lst, cur);
	}
	cur = ft_calloc(1, sizeof (t_token));
	if (!cur || !lst)
		return (free(cur), NULL);
	cur->type = TK_EOF;
	token_addback(&lst, cur);
	return (lst);
}
