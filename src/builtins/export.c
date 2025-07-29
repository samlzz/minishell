/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/29 06:54:37 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "builtins.h"
#include "utils.h"

static inline bool	_print_env_var(char *envvar)
{
	char	*eq;
	char	save;
	bool	ok;

	eq = ft_strchr(envvar, '=');
	ok = bi_putstr("export", "export ");
	if (eq && ok)
	{
		save = eq[1];
		eq[1] = '\0';
		ok = (ok && bi_putstr("export", envvar));
		eq[1] = save;
		ok = (ok && bi_putstr("export", "\""));
		ok = (ok && bi_putstr("export", eq + 1));
		ok = (ok && bi_putstr("export", "\"\n"));
	}
	else if (ok)
		ok = (ok && bi_putendl("export", envvar));
	return (ok);
}

static inline int32_t	_print_env(t_sh_ctx *ctx)
{
	char	**envp;
	size_t	i;
	bool	ok;

	envp = env_get_envp(ctx->env, NULL, true);
	if (!envp)
		return (perror("minishell: malloc"), 1);
	export_sort_envp(envp);
	ok = true;
	i = -1;
	while (envp[++i] && ok)
		ok = _print_env_var(envp[i]);
	return (ft_splitfree(envp, 0), !ok);
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

	if (check_invalid_opt(&ac, &av))
		return (2);
	if (ac == 0)
		return (_print_env(ctx));
	ret = 0;
	i = -1;
	while (++i < ac)
	{
		if (!is_valid_env_name(av[i], true))
			ret = 1;
		if (!is_valid_env_name(av[i], false) || (!ft_strchr(av[i], '=')
				&& env_get(ctx->env, av[i])))
			continue ;
		if (_append_entry(ctx->env, av[i]))
			return (1);
	}
	return (ret);
}
