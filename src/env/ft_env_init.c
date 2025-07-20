/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 23:29:00 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/20 17:33:25 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
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
	char	tmp[12];

	tmp[0] = 0;
	sh_lvl = env_get(env, "SHLVL");
	if (sh_lvl)
	{
		curr_lvl = ft_atoi(sh_lvl) + 1;
		if (curr_lvl < ENV_SHLVL_MIN)
			ft_strlcat(tmp, "0", 12);
		else if (curr_lvl >= ENV_SHLVL_MAX)
		{
			ft_putstr_fd("minishell: warning: shell level (", 2);
			ft_putnbr_fd(curr_lvl, 2);
			ft_putendl_fd(") too high, resetting to 1", 2);
			ft_strlcat(tmp, "1", 12);
		}
		else
			ft_itoa_str(tmp, curr_lvl);
		return (env_literal_set(env, "SHLVL", tmp));
	}
	return (env_literal_set(env, "SHLVL", "1"));
}

static inline int16_t	_init_pwds(t_env *env)
{
	struct stat	st;
	char		*pwd;
	char		*tmp;

	pwd = env_get(env, "OLDPWD");
	if (!pwd)
	{
		tmp = ft_strdup("OLDPWD");
		if (!tmp)
			return (perror("minishell: env_init: malloc"), 1);
		env_set(env, tmp);
	}
	else if (stat(pwd, &st) || !S_ISDIR(st.st_mode))
		env_rm(env, "OLDPWD");
	pwd = getcwd(NULL, 0);
	if (!pwd)
		perror("shell-init: error retrieving current directory: getcwd: cannot "
			"access parent directories");
	else if (env_literal_set(env, "PWD", pwd))
		return (perror("minishell: env_init: malloc"), free(pwd), 1);
	free(pwd);
	return (0);
}

t_env	*env_init(char *const envp[])
{
	t_env		*env;

	env = ft_calloc(1, sizeof (t_env));
	if (!env)
		return (perror("minishell: env_init: malloc"), NULL);
	if (envp && *envp)
	{
		env->entries = strtab_dup(envp);
		if (!env->entries)
			return (free(env), NULL);
		env->size = strtab_len(env->entries);
		env->cap = env->size;
	}
	if (_init_pwds(env))
		return (ft_splitfree(env->entries, env->size), free(env), NULL);
	if (_init_shlvl(env))
		return (perror("minishell: env_init: malloc"),
		ft_splitfree(env->entries, env->size), free(env), NULL);
	return (env);
}
