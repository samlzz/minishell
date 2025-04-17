/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:58:42 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/17 23:42:02 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <dirent.h>
#include <stdlib.h>

static char **_get_pat_segments(t_argword *pat)
{
	char	**segments;
	size_t	prev;
	size_t	i;

	segments = malloc((pat->wild_offsets.len + 2) * sizeof (char *));
	if (!segments)
		return (NULL);
	i = 0;
	prev = 0;
	while (i < pat->wild_offsets.len)
	{
		segments[i] = ft_substr(pat->value, prev, pat->wild_offsets.data[i] + 1);
		prev = pat->wild_offsets.data[i++] + 1;
	}
	if (pat->value[i])
		segments[i++] = ft_substr(pat->value, prev, ft_strlen(pat->value));
	segments[i] = NULL;
	return (segments);
}

static bool	_match_pattern(t_argword *pat, const char *str)
{
	char	**segments;
	char	*cursor;
	size_t	i;
	size_t	len;
	
	cursor = (char *)str;
	segments = _get_pat_segments(pat);
	if (!segments)
		return (false);
	i = 0;
	while (segments[i])
	{
		len = ft_strlen(segments[i]);
		cursor = ft_strnstr(cursor, segments[i], len);
		if (!cursor)
			return (ft_splitfree(segments, 0), false);
		cursor += len;
		i++;
	}
	ft_splitfree(segments, 0);
	return (true);
}

static inline struct dirent	*_init_vars(DIR **dir_ptr, t_argword **null_ptr)
{
	*dir_ptr = opendir(".");
	if (!*dir_ptr)
		return (NULL);
	*null_ptr = NULL;
	return (readdir(*dir_ptr));
}

t_argword	*expand_wildcards(t_argword *arg)
{
	DIR				*dir;
	struct dirent	*strm;
	t_argword		*match;
	t_argword		*new;
	bool			is_hide;

	strm = _init_vars(&dir, &match);
	while (strm)
	{
		is_hide = *strm->d_name == '.' && *arg->value != '.';
		if (!is_hide && _match_pattern(arg, strm->d_name))
		{
			new = argword_new();
			if (!new)
				return (argword_clear(match), NULL);
			argword_add_back(&match, new);
			new->value = ft_strdup(strm->d_name);
			if (!new->value)
				return (argword_clear(match), NULL);
		}
		strm = readdir(dir);
	}
	if (dir)
		closedir(dir);
	return (match);
}
