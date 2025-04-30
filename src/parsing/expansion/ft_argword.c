/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argword.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:25:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 16:54:25 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <stdlib.h>

/**
 * @brief Allocate and initialize a new argword node.
 * 
 * Initializes dynamic arrays for space and wildcard offsets.
 * 
 * @return t_argword* New node or NULL on allocation failure.
 */
t_argword	*argword_new(void)
{
	t_argword	*new;

	new = ft_calloc(1, sizeof (t_argword));
	if (!new)
		return (NULL);
	new->wild_offsets = ft_dynint_new(0);
	if (!new->wild_offsets.data)
	{
		free(new);
		return (NULL);
	}
	new->space_offsets = ft_dynint_new(0);
	if (!new->space_offsets.data)
	{
		ft_dynint_free(&new->wild_offsets);
		free(new);
		return (NULL);
	}
	return (new);
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
int16_t	argword_append_value(t_argword *node, const char *cur_arg, \
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

void	argword_add_back(t_argword **lst, t_argword *new)
{
	t_argword	*last;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
}

void	argword_clear(t_argword *lst)
{
	t_argword	*next;

	while (lst)
	{
		next = lst->next;
		ft_dynint_free(&lst->wild_offsets);
		ft_dynint_free(&lst->space_offsets);
		free(lst->value);
		free(lst);
		lst = next;
	}
}
