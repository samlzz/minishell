/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:00:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/09 18:35:18 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "ft_dynbuf.h"
#include <stdlib.h>

static inline int32_t	_put_value(t_dynbuf *buf, const char *input)
{
	int32_t	i;
	char	*key;
	char	*val;

	i = 0;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	key = ft_substr(input, 0, i);
	if (!key)
		return (-1);
	val = getenv(key);
	free(key);
	if (val && !ft_dynbuf_append_str(buf, val))
		return (-1);
	return (i);
}

static inline bool	_fill_buffer(const char **input, t_dynbuf *buf, char *last_exit)
{
	int32_t	read;

	if (**input == '$')
	{
		(*input)++;
		if (**input == '?')
		{
			(*input)++;
			if (!ft_dynbuf_append_str(buf, last_exit))
				return (true);
		}
		else if (ft_isalpha(**input) || **input == '_')
		{
			read = _put_value(buf, *input);
			if (read == -1)
				return (true);
			*input += read;
		}
		else if (!ft_dynbuf_append_char(buf, '$'))
			return (true);
	}
	else if (!ft_dynbuf_append_char(buf, *(*input)++))
		return (true);
	return (false);
}

static char *_expand_value(const char *input, int16_t last_exit)
{
	t_dynbuf	buf;
	char		*exit_code;

	buf = ft_dynbuf_new(NULL);
	if (!buf.data)
		return (NULL);
	exit_code = ft_itoa(last_exit);
	if (!exit_code)
		return (ft_dynbuf_free(&buf), NULL);
	while (*input)
	{
		if (_fill_buffer(&input, &buf, exit_code))
		{
			free(exit_code);
			ft_dynbuf_free(&buf);
			return (NULL);
		}
	}
	free(exit_code);
	return (buf.data);
}

char	*expand_and_join_words(t_token **cur, int16_t last_exit)
{
	char	*result;
	char	*tmp;
	char	*expanded;

	result = NULL;
	while (*cur && (*cur)->type == TK_WORD)
	{
		if ((*cur)->quote == QUOTE_SINGLE || !ft_strcmp((*cur)->value, "$"))
			expanded = ft_strdup((*cur)->value);
		else
			expanded = _expand_value((*cur)->value, last_exit);
		if (!expanded)
			return (free(result), NULL);
		tmp = ft_strappend(result, expanded);
		(free(result), free(expanded));
		if (!tmp)
			return (NULL);
		result = tmp;
		*cur = (*cur)->next;
		if (*cur && !(*cur)->glued)
			break ;
	}
	return (result);
}
