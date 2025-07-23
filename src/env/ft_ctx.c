/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctx.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 11:21:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/24 01:07:24 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "env.h"
#include "ast/ast.h"

static inline char	*_ft_getpid(void)
{
	int32_t	errno_;
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
	ctx->env = env_init(envp);
	if (!ctx->env)
		return (free(ctx->p_id), free(ctx), NULL);
	if (!env_get(ctx->env, "PATH"))
		ctx->use_fallback_path = true;
	return (ctx);
}

void	context_free(t_sh_ctx *ctx)
{
	ft_splitfree(ctx->env->entries, ctx->env->size);
	free(ctx->env);
	free(ctx->p_id);
	ast_free(ctx->head);
	free(ctx);
}
