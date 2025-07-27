/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/28 00:42:30 by mle-flem         ###   ########.fr       */
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
	bool	ok;

	(void) ac, (void) av;
	envp = env_get_envp(ctx->env, NULL, false);
	if (!envp)
		return (1);
	ok = true;
	i = -1;
	while (envp[++i] && ok)
		ok = (ok && bi_putendl("env", envp[i]));
	ft_splitfree(envp, 0);
	return (!ok);
}
