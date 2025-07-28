/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:43:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/28 17:36:43 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "expander.h"
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
