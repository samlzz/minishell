/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/24 10:56:13 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>

#include "libft.h"
#include "builtins.h"

int32_t	main_env(int32_t ac, char **av, t_sh_ctx *ctx)
{
	size_t	i;
	char	**envp;

	(void) ac, (void) av;
	envp = env_get_envp(ctx->env, NULL, false);
	if (!envp)
		return (1);
	i = -1;
	while (envp[++i])
		ft_putendl_fd(envp[i], STDOUT_FILENO);
	ft_splitfree(envp, 0);
	return (0);
}
