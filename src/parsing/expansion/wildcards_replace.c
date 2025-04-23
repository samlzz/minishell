/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_replace.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:58:12 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/23 13:33:23 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static inline t_argword	*_detach_next(t_argword *node)
{
	t_argword	*next;

	next = node->next;
	node->next = NULL;
	return (next);
}

static inline t_argword	**_insert_node(t_argword **cur,
	t_argword *next_cur, t_argword *node)
{
	t_argword	*tail;

	(*cur)->next = NULL;
	argword_clear(*cur);
	*cur = node;
	tail = node;
	while (tail->next)
		tail = tail->next;
	tail->next = next_cur;
	return (&tail->next);
}

t_argword	*replace_by_wild_expanded(t_argword *head)
{
	t_argword	**cursor;
	t_argword	*expanded;
	t_argword	*next;

	cursor = &head;
	while (*cursor)
	{
		if ((*cursor)->wild_offsets.len == 0)
		{
			cursor = &(*cursor)->next;
			continue ;
		}
		next = _detach_next(*cursor);
		expanded = expand_wildcards(*cursor);
		if (!expanded)
		{
			(*cursor)->next = next;
			cursor = &(*cursor)->next;
			continue ;
		}
		cursor = _insert_node(cursor, next, expanded);
	}
	return (head);
}
