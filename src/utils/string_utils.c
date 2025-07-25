/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:13:30 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/22 21:21:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

bool	ft_isspace(int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	return (
		uc == ' ' || uc == '\f' \
		|| uc == '\n' || uc == '\r' \
		|| uc == '\t' || uc == '\v'
	);
}

char	*ft_str3join(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*ret;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	ret = ft_strjoin(tmp, s3);
	free(tmp);
	return (ret);
}

bool	ft_strjreplace(char **target, const char *suff)
{
	char	*tmp;

	tmp = ft_strappend(*target, suff);
	if (!tmp)
		return (true);
	free(*target);
	*target = tmp;
	return (false);
}
