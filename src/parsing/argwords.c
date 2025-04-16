/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argwords.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:25:33 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/16 14:50:36 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

t_argword	*argword_new(void)
{
	t_argword	*new;

	new = ft_calloc(1, sizeof (t_argword));
	if (!new)
		return (NULL);
	new->wild_offsets = ft_dynint_new(0);
	if (!new->wild_offsets.data)
	{
		free(new);
		return (NULL);
	}
	return (new);
}

int16_t	argword_append_value(t_argword *node, const char *cur_arg, \
	t_quote_type cur_quote)
{
	char	*tmp;
	size_t	offset;

	tmp = ft_strappend(node->value, cur_arg);
	if (!tmp)
		return (0);
	free(node->value);
	node->value = tmp;
	if (cur_quote == QUOTE_NONE && ft_strchr(cur_arg, '*'))
	{
		offset = 0;
		while (node->value[offset] && node->value[offset] != '*')
			offset++;
		if (!node->value[offset] || \
			!ft_dynint_append(&node->wild_offsets, offset))
			return (0);
	}
	return (1);
}

void	argword_add_back(t_argword **lst, t_argword *new)
{
	t_argword	*last;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
}

char	**argwords_to_argv(t_argword *lst)
{
	t_argword	*start;
	size_t		size;
	char		**argv;

	start = lst;
	size = 0;
	while (lst && ++size)
		lst = lst->next;
	if (!size)
		return (NULL);
	argv = malloc((size + 1) * sizeof (char *));
	if (!argv)
		return (NULL);
	lst = start;
	size = 0;
	while (lst)
	{
		argv[size++] = lst->value;
		lst->value = NULL;
		lst = lst->next;
	}
	argv[size] = NULL;
	return (argv);
}

void	argword_clear(t_argword *lst)
{
	t_argword	*next;

	while (lst)
	{
		next = lst->next;
		ft_dynint_free(&lst->wild_offsets);
		free(lst->value);
		free(lst);
		lst = next;
	}
}
