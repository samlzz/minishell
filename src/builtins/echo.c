/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/24 10:55:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "libft.h"
#include "builtins.h"

static inline bool	_check_n(int32_t ac, char **av, int32_t *i)
{
	int32_t	j;
	int32_t	k;
	bool	found;

	j = *i;
	found = false;
	while (++j < ac && !ft_strncmp(av[j], "-n", 2))
	{
		k = 2;
		while (av[j][k] == 'n')
			k++;
		if (av[j][k] == '\0')
			found = true;
		else
			break ;
	}
	*i = j - 1;
	return (found);
}

int32_t	main_echo(int32_t ac, char **av, t_sh_ctx *ctx)
{
	int32_t	i;
	bool	has_n;
	bool	first;

	(void) ctx;
	i = 0;
	has_n = _check_n(ac, av, &i);
	first = true;
	while (++i < ac)
	{
		if (!first)
			ft_putstr_fd(" ", STDOUT_FILENO);
		ft_putstr_fd(av[i], STDOUT_FILENO);
		first = false;
	}
	if (!has_n)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
