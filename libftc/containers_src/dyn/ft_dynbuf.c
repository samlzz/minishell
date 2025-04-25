/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dynbuf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 22:13:41 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/14 15:52:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private/ft_dyn_internal.h"
#include <stdlib.h>

t_dynbuf	ft_dynbuf_new(size_t init_cap)
{
	size_t	cap;

	cap = sizeof (char);
	if (init_cap)
		cap *= init_cap;
	else
		cap *= DYN_INIT_CAP;
	return (
		char_from_generic(_dyn_init(cap))
	);
}

char	*ft_dynbuf_append_char(t_dynbuf *buf, char c)
{
	t_dyn	generic;

	if (buf->len + 1 >= buf->cap)
	{
		generic = char_to_generic(*buf);
		if (!_dyn_resize(&generic, buf->len + (1 * DYN_REALOC_FACTOR), \
			sizeof (char)))
			return (NULL);
		*buf = char_from_generic(generic);
	}
	buf->data[buf->len++] = c;
	buf->data[buf->len] = '\0';
	return (buf->data);
}

char	*ft_dynbuf_append_str(t_dynbuf *buf, const char *s)
{
	t_dyn	generic;
	size_t	slen;

	if (!s)
		return (buf->data);
	slen = ft_strlen(s);
	if (buf->len + slen >= buf->cap)
	{
		generic = char_to_generic(*buf);
		if (!_dyn_resize(&generic, buf->len + slen + 1, sizeof (char)))
			return (NULL);
		*buf = char_from_generic(generic);
	}
	ft_memmove(buf->data + buf->len, s, slen);
	buf->len += slen;
	buf->data[buf->len] = '\0';
	return (buf->data);
}

void	ft_dynbuf_free(t_dynbuf *buf)
{
	t_dyn	generic;

	generic = char_to_generic(*buf);
	_dyn_free(&generic);
}
