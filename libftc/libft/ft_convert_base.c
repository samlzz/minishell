/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:44:45 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/11 13:19:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "private/libft_internal.h"
#include <limits.h>
#include <stdlib.h>

static bool	_is_in_base(char c, char *base, int32_t *index)
{
	int32_t	i;

	i = 0;
	while (base[i])
	{
		if (c == base[i])
		{
			*index = i;
			return (true);
		}
		i++;
	}
	return (false);
}

int32_t	ft_atoi_base(char *str, char *base)
{
	int64_t	r;
	size_t	b_len;
	int32_t	sign;
	int32_t	b_index;
	int32_t	i;

	if (!ft_valid_base(base, &b_len))
		return (0);
	i = 0;
	sign = 1;
	r = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	while (str[i] && (str[i] == '+' || str[i] == '-'))
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (_is_in_base(str[i], base, &b_index))
	{
		r = r * b_len + b_index;
		i++;
	}
	return (r * sign);
}

char	*ft_itoa_base(int32_t n, char *base)
{
	t_mem	*dest;
	size_t	b_len;
	char	*n_in_b;

	if (!ft_valid_base(base, &b_len))
		return (NULL);
	dest = _init_dest_to_convert((int64_t)n, b_len);
	if (!dest)
		return (NULL);
	_nb_to_asciistr((int64_t)n, dest, base, b_len);
	n_in_b = dest->content;
	free(dest);
	return (n_in_b);
}

char	*ft_ltoa_base(int64_t n, char *base)
{
	t_mem	*dest;
	size_t	b_len;
	char	*n_in_b;

	if (!ft_valid_base(base, &b_len))
		return (NULL);
	if (n == LONG_MIN)
	{
		return (ft_strdup("-9223372036854775808"));
	}
	dest = _init_dest_to_convert(n, b_len);
	if (!dest)
		return (NULL);
	_nb_to_asciistr(n, dest, base, b_len);
	n_in_b = dest->content;
	free(dest);
	return (n_in_b);
}

char	*ft_ulltoa_base(uint64_t n, char *base)
{
	uint64_t	tmp;
	size_t		base_len;
	t_mem		dest;

	if (!ft_valid_base(base, &base_len))
		return (NULL);
	tmp = n;
	dest.size = 1;
	while (tmp >= (uint64_t)base_len)
	{
		tmp /= (uint64_t)base_len;
		dest.size++;
	}
	dest.content = malloc(sizeof(char) * (dest.size + 1));
	if (!dest.content)
		return (NULL);
	dest.content[dest.size] = '\0';
	while (dest.size)
	{
		dest.content[--dest.size] = base[n % base_len];
		n /= base_len;
	}
	return (dest.content);
}
