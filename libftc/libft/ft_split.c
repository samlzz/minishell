/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:26:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/08 22:09:13 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdbool.h>

void	ft_splitfree(char **splited, size_t end)
{
	size_t	i;

	i = 0;
	if (end)
	{
		while (i < end)
			free(splited[i++]);
	}
	else
	{
		while (splited[i])
			free(splited[i++]);
	}
	free(splited);
}

static size_t	_count_words(const char *s, char separator)
{
	size_t	i;
	size_t	count;

	if (!s)
		return (-1);
	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == separator)
			i++;
		if (s[i])
			count++;
		while (s[i] != separator && s[i])
			i++;
	}
	return (count);
}

static bool	_get_substr(char const *src, char c, size_t *start, char **substr)
{
	char	*tmp;
	size_t	end;
	size_t	i;

	i = *start;
	while (src[i] == c)
		i++;
	*start = i;
	while (src[i] != c && src[i])
		i++;
	end = i;
	if (end > *start)
	{
		tmp = ft_substr(src, ((uint32_t)(*start)), end - *start);
		if (!tmp)
			return (false);
		*substr = tmp;
	}
	*start = end;
	return (true);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	char	*str;
	size_t	word_c;
	size_t	i;
	size_t	j;

	word_c = _count_words(s, c);
	tab = malloc(sizeof (char *) * (word_c + 1));
	if (!tab)
		return (NULL);
	j = 0;
	i = 0;
	while (s[i] && j < word_c)
	{
		if (_get_substr(s, c, &i, &str))
			tab[j++] = str;
		else
			return (ft_splitfree(tab, j), NULL);
	}
	tab[j] = NULL;
	return (tab);
}
