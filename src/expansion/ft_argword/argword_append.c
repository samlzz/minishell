/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argword_append.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:19:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/28 17:23:45 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "argword.h"
#include "utils.h"

#ifdef MINISHELL_BONUS

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
static inline int16_t	_check_dyn_array(t_argword *node, const char *arg)
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

#else

/**
 * @brief Check for spaces in the argument and record offsets.
 * 
 * This is use to later know witch space need to be splitted 
 * 
 * @param node Target argword node.
 * @param arg Input argument string.
 * @return int16_t 1 on success, 0 on failure.
 * 
 * @note This only runs for unquoted text.
 */
static inline int16_t	_check_dyn_array(t_argword *node, const char *arg)
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
		i++;
	}
	return (1);
}

#endif

/**
 * @brief Append a new value segment to an argword node.
 * 
 * If not quoted, updates space and, for bonus only
 * wildcard tracking offsets.
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
		if (!_check_dyn_array(node, cur_arg))
			return (0);
	}
	new_arg = ft_strappend(node->value, cur_arg);
	if (!new_arg)
		return (0);
	free(node->value);
	node->value = new_arg;
	return (1);
}
