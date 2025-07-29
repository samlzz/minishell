/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 10:11:01 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/29 06:54:00 by mle-flem         ###   ########.fr       */
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

static int32_t	_cmp_env_keys(char *s1, char *s2)
{
	char	*eq1;
	char	*eq2;
	char	save1;
	char	save2;
	int32_t	ret;

	eq1 = ft_strchr(s1, '=');
	if (!eq1)
		eq1 = s1 + ft_strlen(s1);
	save1 = *eq1;
	*eq1 = 0;
	eq2 = ft_strchr(s2, '=');
	if (!eq2)
		eq2 = s2 + ft_strlen(s2);
	save2 = *eq2;
	*eq2 = 0;
	ret = ft_strcmp(s1, s2);
	*eq1 = save1;
	*eq2 = save2;
	return (ret);
}

void	export_sort_envp(char **envp)
{
	size_t	i;
	size_t	j;
	char	*tmp;
	bool	is_sorted;

	i = 0;
	while (envp[i])
		i++;
	while (--i > 0)
	{
		is_sorted = true;
		j = -1;
		while (++j < i)
		{
			if (_cmp_env_keys(envp[j + 1], envp[j]) < 0)
			{
				tmp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = tmp;
				is_sorted = false;
			}
		}
		if (is_sorted)
			return ;
	}
}
