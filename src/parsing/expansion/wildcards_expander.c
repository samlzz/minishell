/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_expander.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:58:42 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 12:41:13 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <dirent.h>
#include <stdlib.h>

#define P 0
#define T 1

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
 * @brief Matches a string `txt` against a wildcard pattern `wpat` using selective wildcards.
 *
 * This function implements a greedy pattern matcher.
 * Only '*' characters located at positions stored in `woffsets` are treated as wildcards.
 * All other '*' are matched literally.
 *
 * @param wpat The wildcard pattern (may include both active and literal '*')
 * @param txt  The target string (typically a filename) to match against the pattern
 * @param woffsets Dynamic array of positions in `wpat` where '*' are active wildcards
 * 
 * @var `p_t[P]` Current index in pattern (`wpat`)
 * @var `p_t[T]` Current index in text (`txt`)
 * @var `lasts[P]` Index of last '*' in `wpat` that was an active wildcard
 * @var `lasts[T]` Index in `txt` where we last encountered an active '*'
 * 
 * If a mismatch occurs and a previous wildcard was active, the function
 * backtracks to the character after the last active '*' and retries.
 * Trailing active wildcards at the end of the pattern are also accepted.
 *
 * @return true if `txt` matches the wildcard pattern `wpat`, false otherwise.
 */
static bool	_match_wilds(const char *wpat, const char *txt, t_dynint woffsets)
{
	int32_t	*p_t;
	int32_t	*lasts;

	p_t = (int32_t [2]){0};
	lasts = (int32_t [2]){-1, -1};
	while (txt[p_t[T]])
	{
		if (wpat[p_t[P]] == '*' && _is_wildcard(woffsets, p_t[P]))
		{
			lasts[P] = p_t[P]++;
			lasts[T] = p_t[T];
		}
		else if (wpat[p_t[P]] == txt[p_t[T]])
			(p_t[P]++, p_t[T]++);
		else if (lasts[P] != -1)
		{
			p_t[P] = lasts[P] + 1;
			p_t[T] = ++lasts[T];
		}
		else
			return (false);
	}
	while (wpat[p_t[P]] == '*' && _is_wildcard(woffsets, p_t[P]))
		p_t[P]++;
	return (wpat[p_t[P]] == '\0');
}

/**
 * @brief Initialize DIR* stream and output pointer for matching.
 *
 * @param dir_ptr Output directory stream pointer.
 * @param null_ptr Output pointer to set to NULL.
 * @return struct dirent* First directory entry or NULL.
 */
static inline struct dirent	*_init_vars(DIR **dir_ptr, t_argword **null_ptr)
{
	*dir_ptr = opendir(".");
	if (!*dir_ptr)
		return (NULL);
	*null_ptr = NULL;
	return (readdir(*dir_ptr));
}

/**
 * @brief Expand a wildcard-containing argword into a list of matched files.
 *
 * Performs matching against the current directory, skipping hidden files unless pattern starts with '.'.
 *
 * @param arg The wildcard argument to expand.
 * @return t_argword* List of matched argwords or NULL on error.
 */
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
	return (argword_sort_alpha(&match), match);
}
