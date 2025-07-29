/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:15:41 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/29 09:08:09 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "expander.h"
#include "ft_dyn.h"
#include "libft.h"

#ifdef MINISHELL_BONUS

/**
 * @brief Adjust wildcard offsets for a substring.
 *
 * Translates wildcard positions from original argword to a split fragment.
 *
 * @param dest Destination offset array.
 * @param src Source offset array.
 * @param start Starting index of substring.
 * @param len Length of the substring.
 * @return int16_t 1 on success, 0 on failure.
 */
static inline int16_t	_adjust_wildcards(t_dynint *dest, t_dynint *src, \
	int32_t start, int32_t len)
{
	size_t	i;

	i = 0;
	while (i < src->len)
	{
		if (src->data[i] >= start && src->data[i] < start + len)
		{
			if (!ft_dynint_append(dest, src->data[i] - start))
				return (0);
		}
		i++;
	}
	return (1);
}

/**
 * @brief Create and append a new argword from a substring.
 *
 * Substring is taken from `arg->value` using [start, start+len).
 *
 * @param lst Destination list to append to.
 * @param arg Source argument word.
 * @param start Start index.
 * @param len Length of substring.
 * @return int16_t 1 on success, 0 on failure.
 */
static int16_t	_add_word(t_argword **lst, t_argword *arg, \
	int32_t start, int32_t len)
{
	t_argword	*new;

	new = argword_new();
	if (!new)
		return (0);
	new->value = ft_substr(arg->value, start, len);
	if (!new->value)
		return (argword_clear(new), 0);
	if (!_adjust_wildcards(&new->wild_offsets, &arg->wild_offsets, start, len))
		return (0);
	new->is_expanded = arg->is_expanded;
	argword_add_back(lst, new);
	return (1);
}

#else

/**
 * @brief Create and append a new argword from a substring.
 *
 * Substring is taken from `arg->value` using [start, start+len).
 *
 * @param lst Destination list to append to.
 * @param arg Source argument word.
 * @param start Start index.
 * @param len Length of substring.
 * @return int16_t 1 on success, 0 on failure.
 */
static int16_t	_add_word(t_argword **lst, t_argword *arg, \
	int32_t start, int32_t len)
{
	t_argword	*new;

	new = argword_new();
	if (!new)
		return (0);
	new->value = ft_substr(arg->value, start, len);
	if (!new->value)
		return (argword_clear(new), 0);
	argword_add_back(lst, new);
	return (1);
}

#endif

static inline bool	_is_full_ws(const char *val, t_dynint space_offset)
{
	int32_t	prev;
	size_t	ws_char;
	size_t	i;

	i = 0;
	ws_char = 0;
	prev = -1;
	while (i < space_offset.len)
	{
		if (space_offset.data[i] == prev + 1)
			ws_char++;
		prev = space_offset.data[i++];
	}
	return (ws_char == ft_strlen(val));
}

/**
 * @brief Split an argument word into several at whitespace positions.
 *
 * Keeps track of original wildcards per segment.
 *
 * @param arg Original argword.
 * @return t_argword* List of split argwords or NULL.
 */
t_argword	*field_splitting(t_argword *field)
{
	t_argword	*lst;
	size_t		i;
	int32_t		len;
	int32_t		start;

	i = 0;
	start = 0;
	lst = NULL;
	if (!field->value || !*field->value \
		|| _is_full_ws(field->value, field->space_offsets)
	)
		return (_add_word(&lst, field, 0, 0), lst);
	while (i < field->space_offsets.len)
	{
		len = field->space_offsets.data[i] - start;
		if (len > 0 && !_add_word(&lst, field, start, len))
			return (argword_clear(lst), NULL);
		start = field->space_offsets.data[i++] + 1;
	}
	if (field->value[start] && \
		!_add_word(&lst, field, start, ft_strlen(field->value) - start))
	{
		return (argword_clear(lst), NULL);
	}
	return (lst);
}
