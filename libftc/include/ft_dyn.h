/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dyn.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:47:18 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/14 15:53:11 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DYN_H
# define FT_DYN_H

# include "libft.h"

# ifndef DYN_INIT_CAP
#  define DYN_INIT_CAP		8
# endif
# ifndef DYN_REALOC_FACTOR
#  define DYN_REALOC_FACTOR	2
# endif

//* Dynbuff (char *)

typedef struct s_dynbuf
{
	char	*data;
	size_t	len;
	size_t	cap;
}	t_dynbuf;

t_dynbuf	ft_dynbuf_new(size_t init_cap);

char		*ft_dynbuf_append_str(t_dynbuf *buf, const char *s)
			__attribute__ ((nonnull(1)));

char		*ft_dynbuf_append_char(t_dynbuf *buf, char c)
			__attribute__ ((nonnull(1)));

void		ft_dynbuf_free(t_dynbuf *buf)
			__attribute__ ((nonnull(1)));

//* Dynint (int *)

typedef struct s_dynint
{
	int32_t	*data;
	size_t	len;
	size_t	cap;
}	t_dynint;

t_dynint	ft_dynint_new(size_t init_cap);

int16_t		ft_dynint_append(t_dynint *buf, int32_t value)
			__attribute__ ((nonnull(1)));

void		ft_dynint_free(t_dynint *buf)
			__attribute__ ((nonnull(1)));

#endif
