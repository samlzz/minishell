/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:43:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/24 10:49:32 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "utils.h"
#include "expansion/expander.h"

t_argword	*argword_detach_next(t_argword *node)
{
	t_argword	*next;

	next = node->next;
	node->next = NULL;
	return (next);
}

t_argword	**argword_insert(t_argword **cur, t_argword *next, t_argword *node)
{
	t_argword	*tail;

	(*cur)->next = NULL;
	argword_clear(*cur);
	*cur = node;
	tail = node;
	while (tail->next)
		tail = tail->next;
	tail->next = next;
	return (&tail->next);
}

/**
 * @brief Check for spaces and wildcards in the argument and record offsets.
 * 
 * This is use to later know witch char need to be expanded 
 * (or splited for space).
 * 
 * @param node Target argword node.
 * @param arg Input argument string.
 * @return int16_t 1 on success, 0 on failure.
 * 
 * @note This only runs for unquoted text.
 */
static inline int16_t	_check_split_and_wild(t_argword *node, const char *arg)
{
	size_t	val_len;
	size_t	i;

	i = 0;
	if (node->value)
		val_len = ft_strlen(node->value);
	else
		val_len = 0;
	while (arg[i])
	{
		if (ft_isspace(arg[i]))
		{
			if (!ft_dynint_append(&node->space_offsets, val_len + i))
				return (0);
		}
		if (arg[i] == '*')
		{
			if (!ft_dynint_append(&node->wild_offsets, val_len + i))
				return (0);
		}
		i++;
	}
	return (1);
}

/**
 * @brief Append a new value segment to an argword node.
 * 
 * If not quoted, updates space and wildcard tracking offsets.
 * 
 * @param node Target argword.
 * @param cur_arg The value string to append.
 * @param cur_quote Quote context of the string.
 * @return int16_t 1 on success, 0 on failure.
 */
int16_t	argword_append_value(t_argword *node, const char *cur_arg,
	t_quote_type cur_quote)
{
	char	*new_arg;

	if (cur_quote == QUOTE_NONE)
	{
		if (!_check_split_and_wild(node, cur_arg))
			return (0);
	}
	new_arg = ft_strappend(node->value, cur_arg);
	if (!new_arg)
		return (0);
	free(node->value);
	node->value = new_arg;
	return (1);
}

bool	is_wildcard(t_dynint wild_offsets, int32_t i)
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
