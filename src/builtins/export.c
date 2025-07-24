/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/25 09:33:51 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "builtins.h"

static inline bool	_check_invalid_opt(int32_t *ac, char ***av)
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

static inline bool	_is_valid_env_name(char *name, bool print_err)
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

static inline void	_print_env_var(char *envvar)
{
	char	*eq;
	char	save;

	eq = ft_strchr(envvar, '=');
	ft_putstr_fd("export ", STDOUT_FILENO);
	if (eq)
	{
		save = eq[1];
		eq[1] = '\0';
		ft_putstr_fd(envvar, STDOUT_FILENO);
		eq[1] = save;
		ft_putstr_fd("\"", STDOUT_FILENO);
		ft_putstr_fd(eq + 1, STDOUT_FILENO);
		ft_putstr_fd("\"\n", STDOUT_FILENO);
	}
	else
		ft_putendl_fd(envvar, STDOUT_FILENO);
}

static inline int32_t	_print_env(t_sh_ctx *ctx)
{
	char	**envp;
	size_t	i;

	envp = env_get_envp(ctx->env, NULL, true);
	if (!envp)
		return (perror("minishell: malloc"), 1);
	i = -1;
	while (envp[++i])
		_print_env_var(envp[i]);
	return (ft_splitfree(envp, 0), 0);
}

static inline int32_t	_append_entry(t_env *env, char *entry)
{
	char	*key;
	char	*val;
	char	*eq;
	char	*p;

	eq = ft_strchr(entry, '=');
	p = ft_strchr(entry, '+');
	if (!eq || !p || p != eq - 1)
	{
		val = ft_strdup(entry);
		if (!val || env_set(env, val))
			return (perror("minishell: export: malloc"), free(val), 1);
		return (0);
	}
	*p = '\0';
	key = env_get(env, entry);
	if (!key)
		return (env_literal_set(env, entry, eq + 1));
	val = ft_str3join(entry, key - 1, eq + 1);
	if (!val || env_set(env, val))
		return (perror("minishell: export: malloc"), free(val), 1);
	return (0);
}

int32_t	main_export(int32_t ac, char **av, t_sh_ctx *ctx)
{
	int32_t	i;
	int32_t	ret;

	if (_check_invalid_opt(&ac, &av))
		return (2);
	if (ac == 0)
		return (_print_env(ctx));
	ret = 0;
	i = -1;
	while (++i < ac)
	{
		if (!_is_valid_env_name(av[i], true))
			ret = 1;
		if (!_is_valid_env_name(av[i], false) || (!ft_strchr(av[i], '=')
				&& env_get(ctx->env, av[i])))
			continue ;
		if (_append_entry(ctx->env, av[i]))
			return (1);
	}
	return (ret);
}
