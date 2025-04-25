/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_iter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:29:39 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/08 22:29:09 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strmapi(char const *s, char (*f) (uint32_t, char))
{
	uint32_t	i;
	char		*dest;

	dest = malloc(sizeof (char) * (ft_strlen(s) + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dest[i] = f(i, s[i]);
		i++;
	}
	dest[i] = 0;
	return (dest);
}

void	ft_striteri(char *s, void (*f) (uint32_t, char *))
{
	uint32_t	i;

	i = 0;
	while (*s)
		f(i++, s++);
}
