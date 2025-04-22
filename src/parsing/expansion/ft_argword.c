/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argword.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:25:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/22 15:19:51 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

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
	new->no_quote = true;
	return (new);
}

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
		if (arg[i] == '*')
		{
			if (!ft_dynint_append(&node->wild_offsets, val_len + i))
				return (0);
		}
		if (arg[i] == ' ' || (arg[i] >= 9 && arg[i] <= 13)) 
		{
			if (!ft_dynint_append(&node->space_offsets, val_len + i))
				return (0);
		}
		i++;
	}
	return (1);
}

int16_t	argword_append_value(t_argword *node, const char *cur_arg, \
	t_quote_type cur_quote)
{
	char	*new_arg;

	if (cur_quote == QUOTE_NONE)
	{
		if (!_check_split_and_wild(node, cur_arg))
			return (0);
	}
	else
		node->no_quote = false;
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
