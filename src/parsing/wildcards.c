/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:58:42 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/18 11:23:08 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <dirent.h>
#include <stdlib.h>
//TODO: tmp
#include <stdio.h>

static inline bool	_is_wildcard(t_dynint wild_offsets, int32_t i)
{
	size_t	j;

	j = 0;
	while (j < wild_offsets.len)
	{
		if (wild_offsets.data[j] == i)
			return (true);
		j++;
	}
	return (false);
}

/**
 * @var `p`: current index in `wpat`
 * @var `t`: current index in `txt`
 * @var `last_star_p`: last position of active '*'
 * @var `last_match_t`: position of txt where we encountred active '*'
 * @if `last_star_p` != -1 :
 *    go back one char after last active '*'
 */
static bool	_match_wilds(const char *wpat, const char *txt, t_dynint woffsets)
{
	int32_t	p;
	int32_t	t;
	int32_t	last_star_p;
	int32_t	last_match_t;

	p = 0;
	t = 0;
	last_star_p = -1;
	last_match_t = -1;
	while (txt[t])
	{
		if (wpat[p] == '*' && _is_wildcard(woffsets, p))
		{
			last_star_p = p++;
			last_match_t = t;
		}
		else if (wpat[p] == txt[t])
			(p++, t++);
		else if (last_star_p != -1)
		{
			p = last_star_p + 1;
			t = ++last_match_t;
		}
		else
			return (false);
	}
	while (wpat[p] == '*' && _is_wildcard(woffsets, p))
		p++;
	return (wpat[p] == '\0');
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
		printf("MATCH? %s ~ %s → %s\n", arg->value, strm->d_name, \
			_match_wilds(arg->value, strm->d_name, arg->wild_offsets) ? "true" : "false");
		if (!is_hide && _match_wilds(arg->value, strm->d_name, arg->wild_offsets))
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
