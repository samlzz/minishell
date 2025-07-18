/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:29:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/18 12:25:55 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t	strtab_len(char *const *tab)
{
	size_t	i;
	
	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

char	**strtab_dup(char * const tab[])
{
	int32_t	i;
	char	**dest;
	char	*tmp;

	dest = malloc((strtab_len(tab) + 1) * sizeof (char *));
	if (!dest)
		return (perror("minishell: strtab_dup: malloc"), NULL);
	i = 0;
	while (tab[i])
	{
		tmp = ft_strdup(tab[i]);
		if (!tmp)
		{
			perror("minishell: strtab_dup: ft_strdup");
			ft_splitfree(dest, i);
			return (NULL);
		}
		dest[i++] = tmp;
	}
	dest[i] = NULL;
	return (dest);
}

static inline t_env	*_env_init_from_envp(char * const envp[])
{
	t_env	*env;

	env = ft_calloc(1, sizeof (t_env));
	if (!env)
		return (perror("minishell: context_init: malloc"), NULL);
	env->entries = strtab_dup(envp);
	if (!env->entries)
		return (free(env), NULL);
	env->size = strtab_len(env->entries);
	env->cap = env->size;
	return (env);
}

/**
 * @brief Update or set the shell level (SHLVL) in the environment.
 * 
 * Handles overflow or underflow scenarios, and emits a warning if the level is too high.
 * 
 * @param env The environment hash map.
 * @return int16_t 0 on success, 1 on failure.
 */
static inline int16_t	_init_shlvl(t_env *env)
{
	int32_t	curr_lvl;
	char	*sh_lvl;

	sh_lvl = env_get(env, "SHLVL");
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
		if (!sh_lvl || env_literal_set(env, "SHLVL", sh_lvl))
			return (perror("minishell: _init_shlvl: malloc"), free(sh_lvl), 1);
	}
	else
		return (env_literal_set(env, "SHLVL", "1"));
	return (0);
}

t_env	*env_init(char *const envp[])
{
	char	*pwd;
	t_env	*env;

	env = NULL;
	if (envp && *envp)
		env = _env_init_from_envp(envp);
	else
		env = ft_calloc(1, sizeof (t_env));
	if (!env)
		return (perror("minishell: env_init: malloc"), NULL);
	if (_init_shlvl(env))
		return (ft_splitfree(env->entries, env->size), free(env), NULL);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		perror("shell-init: error retrieving current directory: getcwd: cannot "
			"access parent directories");
	else if (env_literal_set(env, "PWD", pwd))
		return (perror("minishell: env_init: malloc"),
			ft_splitfree(env->entries, env->size), free(env), free(pwd), NULL);
	free(pwd);
	return (env);
}
