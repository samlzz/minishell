/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:02:03 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/19 18:46:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
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
		_set_hmap_item(env, ENV_PATH_NM, ENV_DEFAULT_PATH)
	);
}

static inline int16_t	_init_shlvl(t_hmap *env)
{
	int32_t	curr_lvl;
	char	*sh_lvl;

	sh_lvl = ft_hmap_get(env, "SHLVL");
	if (sh_lvl)
	{
		curr_lvl = ft_atoi(sh_lvl) + 1;
		if (curr_lvl < ENV_SHLVL_MIN)
			sh_lvl = ft_strdup("0");
		else if (curr_lvl >= ENV_SHLVL_MAX)
		{
			ft_putstr_fd("minishell: warning: shell level (", 2);
			ft_putnbr_fd(curr_lvl, 2);
			ft_putendl_fd(") too high, resetting to 1", 2);
			sh_lvl = ft_strdup("1");
		}
		else
			sh_lvl = ft_itoa(curr_lvl);
		if (!sh_lvl || ft_hmap_set(env, "SHLVL", sh_lvl, free))
			return (1);
	}
	else
		return (_set_hmap_item(env, "SHLVL", "1"));
	return (0);
}

t_hmap	env_init(char **envp, const char *argv0)
{
	t_hmap	env;

	env = ft_hmap_new(NULL);
	if (!env.__entries)
		return (env);
	if (_set_hmap_item(&env, ENV_PRGM_NM, argv0))
		ft_hmap_free(&env, free);
	else if (!envp || !*envp)
	{
		if (_env_minimal_init(&env))
			ft_hmap_free(&env, free);
	}
	else
	{
		if (_env_init_from_envp(envp, &env))
			ft_hmap_free(&env, free);
		else if (_init_shlvl(&env))
			ft_hmap_free(&env, free);
	}
	return (env);
}
