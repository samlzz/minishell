/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:30:32 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/26 19:28:22 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "utils/utils.h"
#include <stdlib.h>

static char	*_join_key_value(char *key, void *value)
{
	return (ft_str3join(key, "=", value));
}

char	**get_envp(t_hmap *env, char *cmd)
{
	char	**envp;
	char	**tmp;
	size_t	len;

	tmp = ft_hmap_to_array(env, &_join_key_value);
	if (!tmp)
		return (NULL);
	len = 0;
	while (tmp[len])
		len++;
	envp = ft_calloc(len + 2, sizeof(char *));
	if (!envp)
		return (ft_splitfree(tmp, 0), NULL);
	len = -1;
	while (tmp[++len])
		envp[len] = tmp[len];
	envp[len] = ft_calloc(ft_strlen(cmd) + 3, sizeof(char));
	if (!envp[len])
		return (ft_splitfree(tmp, 0), free(envp), NULL);
	ft_strlcat(envp[len], "_=", ft_strlen(cmd) + 3);
	ft_strlcat(envp[len], cmd, ft_strlen(cmd) + 3);
	return (envp);
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

void	context_free(t_sh_ctx *ctx)
{
	free(ctx->p_id);
	ft_hmap_free(&ctx->env, &free);
	free(ctx);
}
