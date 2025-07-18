/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 12:20:49 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/18 12:34:10 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <unistd.h>

int16_t	env_literal_set(t_env *env, const char *key, const char *val)
{
	char	*tmp;

	if (val)
		tmp = ft_str3join(key, "=", val);
	else
		tmp = ft_strdup(key);
	if (!tmp)
		return (1);
	return (env_set(env, tmp));
}

void	env_iter(t_env *env, void (*f)(char *))
{
	size_t	i;

	i = 0;
	while (i < env->size)
		f(env->entries[i++]);
}

void	export_print_entry(char *entry)
{
	char	*eq;
	char	save;

	eq = ft_strchr(entry, '=');
	ft_putstr_fd("export ", STDOUT_FILENO);
	if (eq)
	{
		save = eq[1];
		eq[1] = '\0';
		ft_putstr_fd(entry, STDOUT_FILENO);
		eq[1] = save;
		ft_putstr_fd("\"", STDOUT_FILENO);
		ft_putstr_fd(eq + 1, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
	}
	else
		ft_putendl_fd(entry, STDOUT_FILENO);
}

void	env_print_entry(char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (eq)
		ft_putendl_fd(entry, STDOUT_FILENO);
}

