/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dynint.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 08:28:16 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/28 10:45:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private/ft_dyn_internal.h"
#include <stdlib.h>

t_dynint	ft_dynint_new(size_t init_cap)
{
	size_t	cap;

	if (init_cap)
		cap = init_cap;
	else
		cap = DYN_INIT_CAP;
	return (
		int_from_generic(_dyn_init(cap, sizeof (int32_t)))
	);
}

int16_t	ft_dynint_append(t_dynint *buf, int32_t val)
{
	t_dyn	generic;

	if (buf->len == buf->cap)
	{
		generic = int_to_generic(*buf);
		if (!_dyn_resize(&generic, buf->cap * DYN_REALOC_FACTOR))
			return (0);
		*buf = int_from_generic(generic);
	}
	buf->data[buf->len++] = val;
	return (1);
}

void	ft_dynint_free(t_dynint *buf)
{
	t_dyn	generic;

	generic = int_to_generic(*buf);
	_dyn_free(&generic);
}
