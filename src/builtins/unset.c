/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/19 16:36:05 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdio.h>
#include <unistd.h>

static inline bool	_check_invalid_opt(int32_t *ac, char ***av)
{
	char	errstr[38];

	(*ac)--;
	*av = &(*av)[1];
	if (*ac > 0 && ***av == '-' && (**av)[1] && ft_strcmp(**av, "--"))
	{
		errstr[0] = 0;
		ft_strlcat(errstr, "minishell: unset: -?: invalid option\n", 38);
		errstr[19] = (**av)[1];
		ft_putstr_fd(errstr, STDERR_FILENO);
		return (true);
	}
	if (*ac > 0 && !ft_strcmp(**av, "--"))
	{
		(*ac)--;
		*av = &(*av)[1];
	}
	return (false);
}

static inline bool	_is_valid_env_name(char *name)
{
	size_t	i;

	if (name[0] == '\0' || ft_isdigit(name[0]))
		return (false);
	i = 0;
	while (ft_isalpha(name[i]) || ft_isdigit(name[i]) || name[i] == '_')
		i++;
	return (name[i] == '\0');
}

int32_t	main_unset(int32_t ac, char **av, t_sh_ctx *ctx)
{
	int32_t	i;

	(void) ctx;
	if (_check_invalid_opt(&ac, &av))
		return (2);
	i = -1;
	while (++i < ac)
	{
		if (_is_valid_env_name(av[i]))
			env_rm(ctx->env, av[i]);
		if (!ft_strcmp("PATH", av[i]))
			ctx->use_fallback_path = false;
	}
	return (0);
}
