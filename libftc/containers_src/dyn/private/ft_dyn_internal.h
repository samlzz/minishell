/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dyn_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 08:51:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/14 15:52:43 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DYN_INTERNAL_H
# define FT_DYN_INTERNAL_H

# include "ft_dyn.h"

typedef struct s_dyn
{
	void	*data;
	size_t	len;
	size_t	cap;
}	t_dyn;

t_dyn			_dyn_init(size_t init_cap);

int16_t			_dyn_resize(t_dyn *buf, size_t min_cap, size_t elem_nbytes);

void			_dyn_free(t_dyn *buf);

//* Cast functions

static inline t_dyn	char_to_generic(t_dynbuf buf)
{
	return ((t_dyn){buf.data, buf.len, buf.cap});
}

static inline t_dyn	int_to_generic(t_dynint buf)
{
	return ((t_dyn){buf.data, buf.len, buf.cap});
}

static inline t_dynbuf	char_from_generic(t_dyn buf)
{
	return ((t_dynbuf){(char *)buf.data, buf.len, buf.cap});
}

static inline t_dynint	int_from_generic(t_dyn buf)
{
	return ((t_dynint){(int32_t *)buf.data, buf.len, buf.cap});
}

#endif
