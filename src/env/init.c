/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:29:28 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 12:01:24 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdlib.h>
#include <unistd.h>


/**
 * @brief Initialize environment from envp.
 * 
 * Parses environment variables and populates a hash map.
 * If memory allocation fails or a variable cannot be set, cleanup is performed.
 * 
 * @param envp The environment array from main.
 * @param env The target hash map to populate.
 * @return int16_t 0 on success, 1 on failure.
 */
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
		if (!sep && ++i)
			continue ;
		key = ft_substr(envp[i], 0, sep - envp[i]);
		val = ft_strdup(sep + 1);
		if (!key || !val || env_set(env, key, val) < 0)
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

/**
 * @brief Initialize a minimal environment if envp is NULL or empty.
 * 
 * Sets default values for `PWD`, `SHLVL`, and `PATH`.
 * 
 * @param env The target hash map to populate.
 * @return int16_t 0 on success, 1 on failure.
 */
static int16_t	_env_minimal_init(t_hmap *env)
{
	char	pwd[PATH_MAX];

	if (getcwd(pwd, sizeof(pwd)) && env_literal_set(env, "PWD", pwd))
		return (1);
	return (
		env_literal_set(env, "SHLVL", "1") || \
		env_literal_set(env, ENV_PATH_NM, ENV_DEFAULT_PATH)
	);
}

/**
 * @brief Update or set the shell level (SHLVL) in the environment.
 * 
 * Handles overflow or underflow scenarios, and emits a warning if the level is too high.
 * 
 * @param env The environment hash map.
 * @return int16_t 0 on success, 1 on failure.
 */
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
		if (!sh_lvl || env_set(env, "SHLVL", sh_lvl))
			return (1);
	}
	else
		return (env_literal_set(env, "SHLVL", "1"));
	return (0);
}

/**
 * @brief Public entry point to initialize the shell environment.
 * 
 * Chooses between full or minimal initialization based on envp and stores argv0.
 * 
 * @param envp The environment passed to main.
 * @param argv0 The name of the program (used for internal storage).
 * @return t_hmap The initialized environment map. If an error occurs, an empty map is returned.
 */
t_hmap	env_init(char **envp, const char *argv0)
{
	t_hmap	env;

	env = ft_hmap_new(NULL);
	if (!env.__entries)
		return (env);
	if (env_literal_set(&env, ENV_PRGM_NM, argv0))
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
