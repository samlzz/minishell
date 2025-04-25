/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_alloc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:25:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/08 22:09:19 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include <stdlib.h>

char	*ft_strdup(const char *s)
{
	char	*dest;
	size_t	i;

	dest = malloc(sizeof (char) * (ft_strlen(s) + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_substr(char const *s, uint32_t start, size_t len)
{
	char	*dest;
	size_t	i;
	size_t	m_size;
	size_t	s_len;

	s_len = ft_strlen(s);
	if (start >= s_len || !len)
		return (ft_strdup(""));
	if (s_len - start > len)
		m_size = len;
	else
		m_size = s_len - start;
	dest = malloc(sizeof (char) * (m_size + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (i < m_size && s[start + i])
	{
		dest[i] = s[start + i];
		i++;
	}
	while (i <= m_size)
		dest[i++] = 0;
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	size_t	l1;
	size_t	l2;

	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	dest = malloc(sizeof(char) * (l1 + l2 + 1));
	if (!dest)
		return (NULL);
	ft_strlcpy(dest, s1, l1 + 1);
	ft_strlcpy(dest + l1, s2, l2 + 1);
	return (dest);
}

char	*ft_strappend(char const *base, char const *suf)
{
	char	*dest;

	if (base)
		dest = ft_strjoin(base, suf);
	else
		dest = ft_strdup(suf);
	if (!dest)
		return (NULL);
	return (dest);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	s;
	size_t	end;

	s = 0;
	while (s1[s] && ft_strchr(set, (int32_t)s1[s]))
		s++;
	end = ft_strlen(s1);
	while (end > s && ft_strchr(set, (int32_t)s1[end - 1]))
		end--;
	return (ft_substr(s1, (uint32_t)s, end - s));
}
