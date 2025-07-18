/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:20:49 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/18 23:43:37 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

static inline char	**_env_get_envp_unset_underscore(char **ret)
{
	size_t	i;
	size_t	to_del;

	to_del = -1;
	i = -1;
	while (ret[++i])
		if (!ft_strncmp(ret[i], "_=", 2))
			to_del = i;
	if (to_del == (size_t) -1)
		return (ret);
	i = to_del - 1;
	free(ret[i + 1]);
	while (ret[++i])
		ret[i] = ret[i + 1];
	return (ret);
}

static inline char	**_env_get_envp_set_underscore(char **ret, char *cmd)
{
	size_t	i;

	if (!cmd)
		return (_env_get_envp_unset_underscore(ret));
	i = -1;
	while (ret[++i])
		if (!ft_strncmp(ret[i], "_=", 2))
			return (ret);
	ret[i] = ft_strjoin("_=", cmd);
	if (!ret[i])
		return (perror("minishell: malloc"),
			ft_splitfree(ret, 0), NULL);
	return (ret);
}

char	**env_get_envp(t_env *env, char *cmd)
{
	size_t	i;
	size_t	j;
	char	**ret;

	i = -1;
	j = 0;
	while (++i < env->size)
		if (ft_strchr(env->entries[i], '='))
			j++;
	ret = ft_calloc(j + 3, sizeof(char *));
	if (!ret)
		return (perror("minishell: malloc"), NULL);
	i = -1;
	j = 0;
	while (++i < env->size)
	{
		if (cmd && !ft_strncmp(env->entries[i], "_=", 2))
			ret[j++] = ft_strjoin("_=", cmd);
		else if (ft_strchr(env->entries[i], '=')
				&& ft_strncmp(env->entries[i], "_=", 2))
			ret[j++] = ft_strdup(env->entries[i]);
		if (j > 0 && !ret[j - 1])
			return (perror("minishell: malloc"), ft_splitfree(ret, 0), NULL);
	}
	return (_env_get_envp_set_underscore(ret, cmd));
}
