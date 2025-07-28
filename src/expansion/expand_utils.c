/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:43:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/28 19:19:39 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "expander.h"
#include "libft.h"
#include "utils.h"

#ifdef MINISHELL_BONUS

bool	is_wildcard(t_dynint wild_offsets, int32_t i)
{
	size_t	j;

	j = 0;
	while (j < wild_offsets.len)
	{
		if (wild_offsets.data[j] == i)
			return (true);
		j++;
	}
	return (false);
}

static inline bool	_is_dots(char *filename)
{
	return (ft_strcmp(filename, ".") == 0 || ft_strcmp(filename, "..") == 0);
}

static inline void	_argword_del(t_argword *node)
{
	node->next = NULL;
	argword_clear(node);
}

t_argword	*filter_dot_ones(t_argword *lst)
{
	t_argword	*new_head;
	t_argword	*new_tail;
	t_argword	*next;

	new_head = NULL;
	new_tail = NULL;
	while (lst)
	{
		next = lst->next;
		if (lst->value && _is_dots(lst->value))
			_argword_del(lst);
		else
		{
			lst->next = NULL;
			if (!new_head)
				new_head = lst;
			else
				new_tail->next = lst;
			new_tail = lst;
		}
		lst = next;
	}
	return (new_head);
}

#endif

bool	is_export_cmd(t_token *argv)
{
	char	*argv0;
	bool	resp;

	argv0 = NULL;
	while (argv && argv->value)
	{
		ft_strjreplace(&argv0, argv->value);
		argv = argv->next;
		if (argv && !argv->glued)
			break ;
	}
	resp = ft_strcmp(argv0, "export") == 0;
	free(argv0);
	return (resp);
}
