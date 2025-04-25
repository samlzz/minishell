/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_char.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:19:51 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/08 22:29:40 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int16_t	ft_toupper(int32_t c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

int16_t	ft_tolower(int32_t c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

char	*ft_strchr(const char *s, int32_t c)
{
	uint8_t	uc;

	uc = (uint8_t)c;
	if (uc == '\0')
		return ((char *)s + ft_strlen(s));
	while (*s)
	{
		if (*(uint8_t *)s == uc)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

char	*ft_strrchr(const char *s, int32_t c)
{
	size_t	len;

	len = ft_strlen(s) + 1;
	while (len--)
	{
		if (s[len] == (char)c)
			return ((char *)(s + len));
	}
	return (NULL);
}
