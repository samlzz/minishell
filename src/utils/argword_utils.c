/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argword_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:43:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 17:55:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
 * @brief Merge two sorted argword lists.
 * 
 * @param a First sorted list.
 * @param b Second sorted list.
 * @return t_argword* Merged sorted list.
 */
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

/**
 * @brief Split an argword list into two halves for merge sort.
 * 
 * @param src The source list to split.
 * @param front Output pointer to front half.
 * @param back Output pointer to back half.
 */
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

/**
 * @brief Sort an argword list alphabetically using merge sort.
 * 
 * Modifies the list in-place.
 * 
 * @param head Pointer to the head of the list.
 */
void	argword_sort(t_argword **head)
{
	t_argword	*a;
	t_argword	*b;

	if (!head || !*head || !(*head)->next)
		return ;
	_split(*head, &a, &b);
	argword_sort(&a);
	argword_sort(&b);
	*head = _merge(a, b);
}
