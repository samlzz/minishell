/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctx.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:21:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/17 22:21:42 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

static inline char	*_ft_getpid(void)
{
	int32_t errno_;
	int32_t	fd;
	char	str[12];

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
		return (NULL);
	if (read(fd, str, 11) == -1)
	{
		errno_ = errno;
		close(fd);
		errno = errno_;
		return (NULL);
	}
	close(fd);
	str[11] = '\0';
	return (ft_itoa(ft_atoi(str)));
}

static t_env	*_env_init(char * const envp[])
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

t_sh_ctx	*context_init(char **envp, const char *argv0)
{
	t_sh_ctx	*ctx;

	ctx = ft_calloc(1, sizeof(t_sh_ctx));
	if (!ctx)
		return (perror("minishell: malloc"), NULL);
	ctx->p_name = argv0;
	ctx->p_id = _ft_getpid();
	if (!ctx->p_id)
	{
		perror("minishell: ft_getpid");
		return (free(ctx), NULL);
	}
	ctx->env = _env_init(envp);
	if (!ctx->env)
		return (free(ctx->p_id), free(ctx), NULL);
	if (!env_get(&ctx->env, "PATH"))
		ctx->use_fallback_path = true;
	return (ctx);
}

void	context_free(t_sh_ctx *ctx)
{
	ft_splitfree(ctx->env->entries, ctx->env->size);
	free(ctx->env);
	free(ctx->p_id);
	free(ctx);
}
