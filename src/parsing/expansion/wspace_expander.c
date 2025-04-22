/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wspace_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 18:15:41 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/22 18:37:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static inline int16_t	_copy_local_wildcards(t_dynint *dest, t_dynint *src, \
	int32_t start, int32_t len)
{
	size_t	i;

	i = 0;
	while (i < src->len)
	{
		if (src->data[i] >= start && src->data[i] < start + len)
		{
			if (!ft_dynint_append(dest, src->data[i] - start))
				return (0);
		}
		i++;
	}
	return (1);
}

static int16_t	_add_word(t_argword **lst, t_argword *arg, \
	int32_t start, int32_t len)
{
	t_argword	*new;

	new = argword_new();
	if (!new)
		return (0);
	new->value = ft_substr(arg->value, start, len);
	if (!new->value)
		return (argword_clear(new), 0);
	if (!_copy_local_wildcards(&new->wild_offsets, \
		&arg->wild_offsets, start, len))
		return (0);
	argword_add_back(lst, new);
	return (1);
}

t_argword	*split_withespace(t_argword *arg)
{
	t_argword	*lst;
	size_t		i;
	int32_t		len;
	int32_t		start;

	i = 0;
	start = 0;
	lst = NULL;
	if (!arg->value || !*arg->value)
		return (_add_word(&lst, arg, 0, 0), lst);
	while (i < arg->space_offsets.len)
	{
		len = arg->space_offsets.data[i] - start;
		if (len > 0 && !_add_word(&lst, arg, start, len))
				return (argword_clear(lst), NULL);
		start = arg->space_offsets.data[i++] + 1;
	}
	if (arg->value[start] && \
		!_add_word(&lst, arg, start, ft_strlen(arg->value) - start))
			return (argword_clear(lst), NULL);
	return (lst);
}
