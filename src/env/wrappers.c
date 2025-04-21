/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:30:32 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/21 18:12:20 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdlib.h>

static char	*_join_key_value(char *key, void *value)
{
	char	*kv;
	char	*res;

	kv = ft_strjoin(key, "=");
	if (!kv)
		return (NULL);
	res = ft_strjoin(kv, (char *)value);
	free(kv);
	return (res);
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
