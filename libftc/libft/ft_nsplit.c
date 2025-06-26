/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nsplit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 08:25:24 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/26 09:12:39 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	_count_split(char const *s, char c)
{
	size_t	count;

	count = 0;
	while (s && *s && ft_strchr(s, c))
	{
		s = ft_strchr(s, c);
		s += !!s * !!c;
		count++;
	}
	return (count + 1);
}

char	**ft_nsplit(char const *s, char c)
{
	char	**tab;
	char	*str;
	size_t	len;
	size_t	i;

	len = (_count_split(s, c) + 1) * sizeof(char *) + (ft_strlen(s) + 1 + !c)
		* sizeof(char);
	tab = malloc(len);
	if (!tab)
		return (NULL);
	ft_bzero(tab, len);
	str = ((char *) tab) + (_count_split(s, c) + 1) * sizeof(char *);
	ft_strlcpy(str, s, ft_strlen(s) + 1);
	i = 0;
	tab[i++] = str;
	while (str && *str && ft_strchr(str, c))
	{
		str = ft_strchr(str, c);
		if (str)
			*str++ = 0;
		tab[i++] = str;
	}
	return (tab);
}
