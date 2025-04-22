/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argword_sort.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:26:22 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/22 16:31:49 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static t_argword	*_merge(t_argword *a, t_argword *b)
{
	t_argword *result;

	if (!a)
		return (b);
	if (!b)
		return (a);
	if (ft_strcmp(a->value, b->value) <= 0)
	{
		result = a;
		result->next = _merge(a->next, b);
	}
	else
	{
		result = b;
		result->next = _merge(a, b->next);
	}
	return (result);
}

static void	_split(t_argword *src, t_argword **front, t_argword **back)
{
	t_argword	*fast;
	t_argword	*slow;

	fast = src->next;
	slow = src;
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	*front = src;
	*back = slow->next;
	slow->next = NULL;
}

void	argword_sort_alpha(t_argword **head)
{
	t_argword	*a;
	t_argword	*b;

	if (!head || !*head || !(*head)->next)
		return ;
	_split(*head, &a, &b);
	argword_sort_alpha(&a);
	argword_sort_alpha(&b);
	*head = _merge(a, b);
}
