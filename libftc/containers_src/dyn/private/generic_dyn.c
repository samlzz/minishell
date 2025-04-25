/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic_dyn.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 08:37:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/19 22:00:52 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dyn_internal.h"
#include <stdlib.h>

t_dyn	_dyn_init(size_t init_cap)
{
	t_dyn	buf;

	buf.cap = init_cap;
	buf.data = ft_calloc(buf.cap, 1);
	if (!buf.data)
		return ((t_dyn){0});
	buf.len = 0;
	return (buf);
}

void	_dyn_free(t_dyn *buf)
{
	free(buf->data);
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
}

int16_t	_dyn_resize(t_dyn *buf, size_t min_cap, size_t elem_nbytes)
{
	size_t	new_cap;
	void	*new_data;

	if (min_cap >= SIZE_MAX / elem_nbytes || buf->cap >= SIZE_MAX / 2)
		return (0);
	new_cap = buf->cap * DYN_REALOC_FACTOR;
	while (new_cap < min_cap)
		new_cap *= DYN_REALOC_FACTOR;
	new_data = ft_realloc(buf->data, \
		buf->len * elem_nbytes, new_cap * elem_nbytes);
	if (!new_data)
		return (0);
	free(buf->data);
	buf->data = new_data;
	buf->cap = new_cap;
	return (1);
}
