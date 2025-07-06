/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 07:38:14 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/01 17:44:59 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

bool	get_builtin_func(char *cmd, t_builtin_func *func)
{
	const t_builtin_func	funcs[] = {main_cd, main_echo, main_env, main_exit,
		main_export, main_pwd, main_unset};
	const char				*names[] = {"cd", "echo", "env", "exit", "export",
		"pwd", "unset", NULL};
	size_t					i;

	*func = NULL;
	i = -1;
	while (names[++i])
		if (!ft_strcmp(names[i], cmd))
			*func = funcs[i];
	return (!!*func);
}
