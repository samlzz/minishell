/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/06 05:25:11 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdio.h>

int32_t	main_cd(int32_t ac, char **av, t_sh_ctx *ctx)
{
	int32_t	i;

	(void) ctx;
	printf("Executing cd:\n");
	i = -1;
	while (++i < ac)
		printf("%d:\t%s\n", i, av[i]);
	printf("%d:\t%s\n", i, av[i]);
	return (0);
}
