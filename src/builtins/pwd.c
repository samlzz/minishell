/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/28 00:44:42 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "libft.h"
#include "builtins.h"

static inline bool	_check_invalid_opt(int32_t ac, char **av)
{
	int32_t	i;
	char	erropt;
	char	errstr[36];

	erropt = 0;
	i = 0;
	while (++i < ac && ft_strcmp(av[i], "--"))
	{
		if (av[i][0] == '-' && av[i][1] != '\0')
		{
			erropt = av[i][1];
			break ;
		}
	}
	if (erropt)
	{
		errstr[0] = 0;
		ft_strlcat(errstr, "minishell: pwd: -?: invalid option\n", 36);
		errstr[17] = erropt;
		ft_putstr_fd(errstr, STDERR_FILENO);
	}
	return (!!erropt);
}

int32_t	main_pwd(int32_t ac, char **av, t_sh_ctx *ctx)
{
	char	*pwd;
	bool	ok;

	(void) ctx;
	if (_check_invalid_opt(ac, av))
		return (2);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("pwd: error retrieving current directory: getcwd: " \
			"cannot access parent directories"), 1);
	ok = bi_putendl("pwd", pwd);
	free(pwd);
	return (!ok);
}
