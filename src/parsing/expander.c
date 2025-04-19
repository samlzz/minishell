/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:00:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/19 17:34:23 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "env.h"
#include <stdlib.h>

static int32_t	_put_value(t_hmap *env, t_dynbuf *buf, \
	const char *input)
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
	val = ft_hmap_get(env, key);
	free(key);
	if (val && !ft_dynbuf_append_str(buf, val))
		return (-1);
	return (i);
}

static inline bool	_handle_num(t_hmap *env, t_dynbuf *buf, const char *input)
{
	char	*prgm_name;

	if (*input == '0')
	{
		prgm_name = ft_hmap_get(env, ENV_PRGM_NM);
		if (!prgm_name)
			prgm_name = "minishell";
		return (!ft_dynbuf_append_str(buf, prgm_name));
	}
	return (false);
}

static inline bool	_fill_buffer(t_hmap *env, const char **input, \
	t_dynbuf *buf, char *last_exit)
{
	int32_t	read;

	if (**input == '$')
	{
		(*input)++;
		if (**input == '?')
		{
			(*input)++;
			return (!ft_dynbuf_append_str(buf, last_exit));
		}
		else if (**input >= '0' && **input <= '9')
			return (_handle_num(env, buf, (*input)++));
		else if (ft_isalpha(**input) || **input == '_')
		{
			read = _put_value(env, buf, *input);
			*input += read;
			return (read == -1);
		}
		else
			return (!ft_dynbuf_append_char(buf, '$'));
	}
	else
		return (!ft_dynbuf_append_char(buf, *(*input)++));
}

static char *_expand_value(t_hmap *env, t_token *cur, int16_t last_exit)
{
	t_dynbuf	buf;
	char		*exit_code;
	const char	*input;

	if (cur->quote == QUOTE_SINGLE || !ft_strcmp(cur->value, "$"))
		return (ft_strdup(cur->value));
	input = cur->value;
	buf = ft_dynbuf_new(ft_strlen(cur->value));
	if (!buf.data)
		return (NULL);
	exit_code = ft_itoa(last_exit);
	if (!exit_code)
		return (ft_dynbuf_free(&buf), NULL);
	while (*input)
	{
		if (_fill_buffer(env, &input, &buf, exit_code))
		{
			free(exit_code);
			ft_dynbuf_free(&buf);
			return (NULL);
		}
	}
	free(exit_code);
	return (buf.data);
}

t_argword	*expand_and_join_words(t_hmap *env, t_token **cur, \
	int16_t last_exit)
{
	t_argword	*node;
	char		*expanded;
	int16_t		err;

	node = argword_new();
	if (!node)
		return (NULL);
	while (*cur && (*cur)->type == TK_WORD)
	{
		expanded = _expand_value(env, *cur, last_exit);
		if (!expanded)
			return (argword_clear(node), NULL);
		err = !argword_append_value(node, expanded, (*cur)->quote);
		free(expanded);
		if (err)
			return (argword_clear(node), NULL);
		*cur = (*cur)->next;
		if (*cur && !(*cur)->glued)
			break ;
	}
	return (node);
}
