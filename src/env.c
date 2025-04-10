/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:02:03 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/10 15:56:22 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

static int16_t	_env_init_from_envp(char **envp, t_hmap *env)
{
	int16_t	exit;
	size_t	i;
	char	*sep;
	char	*key;
	char	*val;

	i = 0;
	exit = 0;
	while (envp && envp[i])
	{
		sep = ft_strchr(envp[i], '=');
		if (!sep)
			continue ;
		key = ft_substr(envp[i], 0, sep - envp[i]);
		val = ft_strdup(sep + 1);
		if (!key || !val || ft_hmap_set(env, key, val, free) < 0)
		{
			exit = 1;
			(free(key), free(val));
			ft_hmap_free(env, free);
			break ;
		}
		(free(key), i++);
	}
	return (exit);
}

static inline int16_t	_set_hmap_item(t_hmap *env, const char *key, const char *val)
{
	char	*alloc_val;

	alloc_val = ft_strdup(val);
	if (!alloc_val)
	{
		free(alloc_val);
		return (1);
	}
	if (ft_hmap_set(env, key, alloc_val, free))
	{
		free(alloc_val);
		return (1);
	}
	return (0);
}

static int16_t	_env_minimal_init(t_hmap *env)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, sizeof(pwd)) && _set_hmap_item(env, "PWD", pwd))
		return (1);
	return (
		_set_hmap_item(env, "SHLVL", "1") || \
		_set_hmap_item(env, "_", "/usr/bin/env") || \
		_set_hmap_item(env, "TERM", "xterm-256color")
	);
}

static inline int16_t	_init_shlvl(t_hmap *env)
{
	char	*sh_lvl;

	sh_lvl = ft_hmap_get(env, "SHLVL");
	if (sh_lvl)
	{
		sh_lvl = ft_itoa(ft_atoi(sh_lvl) + 1);
		if (!sh_lvl || ft_hmap_set(env, "SHLVL", sh_lvl, free))
			return (1);
	}
	else
		return (_set_hmap_item(env, "SHLVL", "1"));
	return (0);
}

t_hmap	env_init(char **envp)
{
	t_hmap	env;

	env = ft_hmap_new(NULL);
	if (!env.__entries)
		return (env);
	if (!envp || !*envp)
	{
		if (_env_minimal_init(&env))
			ft_hmap_free(&env, &free);
		else if (_init_shlvl(&env))
			ft_hmap_free(&env, &free);
	}
	else
	{
		if (_env_init_from_envp(envp, &env))
			ft_hmap_free(&env, &free);
	}
	return (env);
}
