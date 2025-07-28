/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:58:42 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/28 19:16:05 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef MINISHELL_BONUS

# include <dirent.h>

# include "expander.h"
# include "utils.h"

# define P 0
# define T 1

static inline void	_reassign_nb(int32_t *nb1, int32_t *nb2,
	int32_t val1, int32_t val2)
{
	*nb1 = val1;
	*nb2 = val2;
}

/**
 * @brief Matches a string `txt` against a wildcard pattern `wpat`
 * using selective wildcards.
 *
 * This function implements a greedy pattern matcher.
 * Only '*' characters located at positions stored in 
 * woffsets` are treated as wildcards, other '*' are matched literally.
 *
 * @param wpat Wildcard pattern
 * @param txt Target string to match against the pattern
 * @param woffsets Dynamic array of positions in `wpat` of active wildcards
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
static bool	_globbing_match(const char *wpat, const char *txt,
	t_dynint woffsets)
{
	int32_t	*p_t;
	int32_t	*lasts;

	p_t = (int32_t [2]){0};
	lasts = (int32_t [2]){-1, -1};
	while (txt[p_t[T]])
	{
		if (wpat[p_t[P]] == '*' && is_wildcard(woffsets, p_t[P]))
			_reassign_nb(&lasts[P], &lasts[T], p_t[P]++, p_t[T]);
		else if (wpat[p_t[P]] == txt[p_t[T]])
		{
			++p_t[P];
			++p_t[T];
		}
		else if (lasts[P] != -1)
			_reassign_nb(&p_t[P], &p_t[T], lasts[P] + 1, ++lasts[T]);
		else
			return (false);
	}
	while (wpat[p_t[P]] == '*' && is_wildcard(woffsets, p_t[P]))
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
 * Performs matching against the current directory, 
 * skipping hidden files unless pattern starts with '.'.
 *
 * @param arg The wildcard argument to expand.
 * @return t_argword* List of matched argwords or NULL on error. 
 * List is sorted by their values.
 * 
 * @see ft_strcmp
 */
static t_argword	*_expand_wildcards(t_argword *arg)
{
	DIR				*dir;
	struct dirent	*strm;
	t_argword		*match;
	t_argword		*new;

	strm = _init_vars(&dir, &match);
	while (strm)
	{
		if (!(*strm->d_name == '.' && *arg->value != '.') \
			&& _globbing_match(arg->value, strm->d_name, arg->wild_offsets))
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
	match = filter_dot_ones(match);
	return (argword_sort(&match), match);
}

/**
 * @brief Replace each argword with wildcard by its expanded list of matches.
 *
 * Preserves nodes without wildcards. On error, fallback to original.
 *
 * @param head Head of argword list.
 * @return t_argword* Updated argword list.
 */
t_argword	*replace_wildcards(t_argword *head)
{
	t_argword	**cursor;
	t_argword	*expanded;
	t_argword	*next;

	cursor = &head;
	while (*cursor)
	{
		if ((*cursor)->wild_offsets.len == 0)
		{
			cursor = &(*cursor)->next;
			continue ;
		}
		next = argword_detach_next(*cursor);
		expanded = _expand_wildcards(*cursor);
		if (!expanded)
		{
			(*cursor)->next = next;
			cursor = &(*cursor)->next;
			continue ;
		}
		cursor = argword_insert(cursor, next, expanded);
	}
	return (head);
}

#endif
