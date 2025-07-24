/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/24 10:56:35 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "builtins.h"
#include "libft.h"
#include "utils.h"

static inline int32_t	_exit_atoi(char **s_)
{
	int32_t	res;
	int32_t	sign;
	char	*s;

	s = *s_;
	while (ft_isspace(*s))
		s++;
	sign = 1;
	if (*s == '-' || *s == '+')
		sign = (*s++ == '+') * 2 - 1;
	if (*s == '\0')
		return (*s = '!', *s_ = s, 0);
	res = 0;
	while (ft_isdigit(*s))
		res = res * 10 + sign * (*s++ - '0');
	while (ft_isspace(*s))
		s++;
	return (*s_ = s, res);
}

int32_t	main_exit(int32_t ac, char **av, t_sh_ctx *ctx)
{
	char	*arg;
	char	*tmp;
	int32_t	ret;
	size_t	arg_idx;

	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (ac == 1 || (ac == 2 && !ft_strcmp(av[1], "--")))
		return (ctx->exit = true, ctx->lst_exit);
	arg_idx = 1 + !ft_strcmp(av[1], "--");
	arg = ft_strdup(av[arg_idx]);
	if (!arg)
		return (perror("minishell: malloc"), 1);
	tmp = arg;
	ret = _exit_atoi(&arg);
	if (*arg != '\0')
		return (ft_putstr_fd("minishell: exit: ", STDERR_FILENO),
			ft_putstr_fd(av[arg_idx], STDERR_FILENO),
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO),
			free(tmp), ctx->exit = true, 2);
	free(tmp);
	if (ac > (2 + !ft_strcmp(av[1], "--")))
		return (ft_putstr_fd("minishell: exit: too many arguments\n",
				STDERR_FILENO), 1);
	return (ctx->exit = true, ret);
}
