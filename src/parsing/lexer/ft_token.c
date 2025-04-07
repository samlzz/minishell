/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:14:26 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/03 12:54:00 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

void	token_clear(t_token *lst)
{
	t_token	*next;

	while (lst)
	{
		if (lst->value)
			free(lst->value);
		next = lst->next;
		free(lst);
		lst = next;
	}
}

t_token	*token_last(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	token_addfront(t_token **lst, t_token *new)
{
	if (new)
	{
		new->next = *lst;
		*lst = new;
	}
}

void	token_addback(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!*lst)
		return (token_addfront(lst, new));
	last = token_last(*lst);
	last->next = new;
}

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
