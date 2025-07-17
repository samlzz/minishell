/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 21:57:43 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/17 23:42:54 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "utils/utils.h"
#include <stdlib.h>

static int32_t	_env_get_idx(t_env *env, const char *key)
{
	char	*eq;
	size_t	i;

	i = 0;
	while (i < env->size)
	{
		eq = ft_strchr(env->entries[i], '=');
		if (!eq && ft_strcmp(env->entries[i], key) == 0)
			return (i);
		if (eq && !ft_strncmp(env->entries[i], key, eq - env->entries[i]) &&
		!key[eq - env->entries[i]])
			return (i);
		i++;
	}
	return (-1);
}

char	*env_get(t_env *env, const char *key)
{
	char	*eq;
	int32_t	idx;

	idx = _env_get_idx(env, key);
	if (idx == -1)
		return (NULL);
	eq = ft_strchr(env->entries[idx], '=');
	if (!eq)
		return (NULL);
	return (eq + 1);
}

static int16_t	_env_resize(t_env *env)
{
	char	**resized;

	if (env->size < env->cap)
		return (0);
	env->cap *= 2;
	resized = ft_realloc(env->entries, env->size * sizeof (char *),
		env->cap * sizeof (char *));
	if (!resized)
		return (env->cap /= 2, 1);
	free(env->entries);
	env->entries = resized;
	return (0);
}

int16_t	env_set(t_env *env, char *entry)
{
	int32_t	idx;
	char	*eq;
	char	*key;

	eq = ft_strchr(entry, '=');
	if (eq)
		key = ft_substr(entry, 0, eq - entry);
	else
		key = ft_strdup(entry);
	if (!key)
		return (1);
	idx = _env_get_idx(env, key);
	free(key);
	if (idx != -1)
	{
		free(env->entries[idx]);
		env->entries[idx] = entry;
		return (0);
	}
	if (_env_resize(env))
		return (1);
	env->entries[env->size++] = entry;
	env->entries[env->size] = NULL;
	return (0);
}

int16_t	env_literal_set(t_env *env, const char *key, const char *val)
{
	char	*tmp;

	if (val)
		tmp = ft_str3join(key, "=", val);
	else
		tmp = ft_strdup(key);
	if (!tmp)
		return (1);
	return (env_set(env, tmp));
}

void	env_rm(t_env *env, const char *key)
{
	size_t	i;
	int32_t	to_del;

	to_del = _env_get_idx(env, key);
	if (to_del == -1)
		return ;
	free(env->entries[to_del]);
	i = 0;
	while (i < env->size)
	{
		env->entries[i] = env->entries[i + 1];
		i++;
	}
	env->entries[i] = NULL;
	env->size--;
}
