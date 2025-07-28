/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argword_lib_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:25:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/28 17:13:56 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef MINISHELL_BONUS

# include <stdlib.h>

# include "argword.h"

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

#endif