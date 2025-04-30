/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:14:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 12:51:20 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include <stdlib.h>

/**
 * @brief Advance a token cursor to the next token.
 *
 * @param cur Pointer to the current token pointer.
 */
void	next(t_token **cur)
{
	*cur = (*cur)->next;
}

void	token_clear(t_token *lst)
{
	t_token	*next;

	while (lst)
	{
		if (lst->value)
			free(lst->value);
		if (lst->unexpanded)
			free(lst->unexpanded);
		next = lst->next;
		free(lst);
		lst = next;
	}
}

void	token_addback(t_token **lst, t_token *new)
{
	t_token	*last;

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

/**
 * @brief Remove and isolate a token from the list.
 *
 * @param lst Pointer to the list head.
 * @param to_retrieve Token to remove.
 * @return t_token* Pointer to removed token, or NULL if not found.
 */
t_token *token_pop(t_token **lst, t_token *to_retrieve)
{
	t_token *prev;
	t_token	*cur;

	prev = NULL;
	cur = *lst;
	while (cur && cur != to_retrieve)
	{
		prev = cur;
		cur = cur->next;
	}
	if (!cur)
		return (NULL);
	if (prev)
		prev->next = cur->next;
	else
		*lst = cur->next;
	to_retrieve->next = NULL;
	return (to_retrieve);
}

/**
 * @brief Duplicate a token while transferring ownership of its strings.
 *
 * Useful for preserving a token’s content while clearing the original.
 *
 * @param og Token to duplicate.
 * @return t_token* New token or NULL.
 */
t_token	*token_dup(t_token *og)
{
	t_token	*copy;

	copy = malloc( sizeof (t_token));
	if (!copy)
		return (NULL);
	copy->next = NULL;
	copy->glued = og->glued;
	copy->quote = og->quote;
	copy->type = og->type;
	copy->value = og->value;
	og->value = NULL;
	copy->unexpanded = og->unexpanded;
	og->unexpanded = NULL;
	return (copy);
}
