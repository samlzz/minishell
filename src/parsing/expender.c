/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expender.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:00:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/04 14:09:55 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

void	ft_strscat(t_mem *buff, size_t *cap, char *to_add)
{
	size_t	len;
	char	*tmp;

	len = ft_strlen(to_add);
	if (!len)
		return ;
	if (buff->size + len + EXP_BUFF_PAD > *cap)
	{
		*cap = *cap + len + EXP_BUFF_PAD;
		tmp = ft_realloc(buff->content, buff->size, *cap);
		if (!tmp)
			return (NULL);
		free(buff->content);
		buff->content = tmp;
	}
	ft_memcpy(buff->content + buff->size, to_add, len);
	buff->size += len;
}

static inline bool	_put_exit_code(t_mem *buff, size_t *cap, int last_exit)
{
	char	*code;

	code = ft_itoa(last_exit);
	if (!code)
		return (false);
	ft_strscat(buff, cap, code);
	free(code);
	return (true);
}

static inline bool	_put_value(char *input, size_t *i, t_mem *buff, size_t *cap)
{
	size_t	start;
	char	*key;
	char	*val;

	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	if (*i == start)
		return (false);
	key = ft_substr(input, start, i - start);
	if (!key)
		return (false);
	val = getenv(key);
	free(key);
	if (val)
		ft_strscat(buff, cap, val);
	return (true);
}

static inline bool	_put_char(t_mem *buff, size_t *cap, char c)
{
	char	*tmp;
	
	if (buff->size + 1 > *cap)
	{
		*cap = buff->size + 1 + EXP_BUFF_PAD;
		tmp = ft_realloc(buff->content, buff->size, *cap);
		if (!tmp)
			return (false);
		buff->content = tmp;
	}
	buff->content[buff->size++] = c;
	return (true);
}

static char *_expand_value(const char *input, int last_exit)
{
	t_mem	buff;
	size_t	cap;
	size_t	i;

	i = 0;
	cap = ft_strlen(input) + EXP_BUFF_PAD;
	buff = (t_mem) {ft_calloc(cap, sizeof (char)), 0};
	if (!buff.content)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '$')
		{
			if (input[++i] == '?' && !_put_exit_code(&buff, &cap, last_exit))
				return (free(buff.content), NULL);
			else if ((ft_isalnum(input[i]) || input[i] == '_') && \
				! _put_value(input, &i, &buff, &cap))
				return (free(buff.content), NULL);
			else
				_put_char(&buff, &cap, input[i++]);
		}
		else
			_put_char(&buff, &cap, input[i++]);
	}
	
}

char	*expand_and_join_words(t_token **cur, t_token **errtok)
{

}
