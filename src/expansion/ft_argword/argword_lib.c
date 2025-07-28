/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argword_lib.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:25:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/28 17:44:40 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "argword.h"

size_t	argword_size(t_argword *head)
{
	size_t	i;

	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}

t_argword	*argword_getlast(t_argword *lst)
{
	while (lst && lst->next)
		lst = lst->next;
	return (lst);
}

void	argword_add_back(t_argword **lst, t_argword *new)
{
	t_argword	*last;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = argword_getlast(*lst);
	last->next = new;
}

#ifndef MINISHELL_BONUS

/**
 * @brief Allocate and initialize a new argword node.
 * 
 * Initializes dynamic array for space offsets.
 * 
 * @return t_argword* New node or NULL on allocation failure.
 */
t_argword	*argword_new(void)
{
	t_argword	*new;

	new = ft_calloc(1, sizeof (t_argword));
	if (!new)
		return (NULL);
	new->space_offsets = ft_dynint_new(0);
	if (!new->space_offsets.data)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

void	argword_clear(t_argword *lst)
{
	t_argword	*next;

	while (lst)
	{
		next = lst->next;
		ft_dynint_free(&lst->space_offsets);
		free(lst->value);
		free(lst);
		lst = next;
	}
}

#endif