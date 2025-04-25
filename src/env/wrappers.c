/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:30:32 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/25 21:20:02 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "parser.h"
#include <stdlib.h>

static char	*_join_key_value(char *key, void *value)
{
	return (ft_str3join(key, "=", value));
}

char	**get_envp(t_hmap *env)
{
	return (ft_hmap_to_array(env, &_join_key_value));
}

int16_t	env_set(t_hmap *env, const char *key, char *value)
{
	return (ft_hmap_set(env, key, value, free));
}

int16_t	env_literal_set(t_hmap *env, const char *key, const char *val)
{
	char	*alloc_val;

	alloc_val = ft_strdup(val);
	if (!alloc_val)
	{
		free(alloc_val);
		return (1);
	}
	if (env_set(env, key, alloc_val))
	{
		free(alloc_val);
		return (1);
	}
	return (0);
}
