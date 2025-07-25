/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:11:01 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 10:11:20 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"

bool	check_invalid_opt(int32_t *ac, char ***av)
{
	char	errstr[39];

	(*ac)--;
	*av = &(*av)[1];
	if (*ac > 0 && ***av == '-' && (**av)[1] && ft_strcmp(**av, "--"))
	{
		errstr[0] = 0;
		ft_strlcat(errstr, "minishell: export: -?: invalid option\n", 39);
		errstr[20] = (**av)[1];
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

bool	is_valid_env_name(char *name, bool print_err)
{
	size_t	i;

	if (name[0] != '\0' && name[0] != '=' && name[0] != '+' \
		&& !ft_isdigit(name[0]))
	{
		i = 0;
		while (ft_isalpha(name[i]) || ft_isdigit(name[i]) || name[i] == '_')
			i++;
		if (name[i] == '\0' || name[i] == '=' \
			|| (name[i] == '+' && name[i + 1] == '='))
			return (true);
	}
	if (!print_err)
		return (false);
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (false);
}
