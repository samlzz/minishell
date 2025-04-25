/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:33:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/11 13:23:39 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft_internal.h"
#include <stdlib.h>

void	_nb_to_asciistr(int64_t nb, t_mem *dest, char *base, size_t b_len)
{
	dest->content[dest->size] = '\0';
	if (nb == 0)
	{
		dest->content[0] = base[0];
		return ;
	}
	if (nb < 0)
	{
		nb = -nb;
		dest->content[0] = '-';
	}
	while (nb)
	{
		dest->content[--dest->size] = base[nb % b_len];
		nb /= b_len;
	}
}

t_mem	*_init_dest_to_convert(int64_t n, size_t base_l)
{
	t_mem	*dest;

	dest = malloc(sizeof (t_mem));
	if (!dest)
		return (NULL);
	dest->size = 0;
	if (n <= 0)
	{
		dest->size = 1;
		if (n)
			n = -n;
	}
	while (n)
	{
		n /= base_l;
		dest->size++;
	}
	dest->content = malloc(sizeof (char) * (dest->size + 1));
	if (!dest->content)
	{
		free(dest);
		return (NULL);
	}
	return (dest);
}

bool	ft_valid_base(char *base, size_t *len)
{
	size_t	i;
	size_t	j;

	if (!base)
		return (false);
	i = 0;
	while (base[i])
	{
		if (base[i] == '-' || base[i] == '+' || base[i] <= ' ')
			return (false);
		j = i + 1;
		while (base[j])
		{
			if (base[i] == base[j])
				return (false);
			j++;
		}
		i++;
	}
	if (i < 2)
		return (false);
	*len = i;
	return (true);
}
