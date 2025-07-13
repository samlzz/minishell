/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/13 10:39:35 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdlib.h>
#include <unistd.h>

static inline bool	_check_invalid_opt(int32_t *ac, char ***av)
{
	char	errstr[35];

	(*ac)--;
	*av = &(*av)[1];
	if (*ac > 0 && ***av == '-' && (**av)[1] && ft_strcmp(**av, "--"))
	{
		errstr[0] = 0;
		ft_strlcat(errstr, "minishell: cd: -?: invalid option\n", 35);
		errstr[16] = (**av)[1];
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

static inline bool	_get_dest_dir(char **dst, int32_t ac, char **av,
																t_sh_ctx *ctx)
{
	char	*user;

	*dst = av[0];
	if (ac == 0)
	{
		user = ft_getuser();
		if (!user)
			return (perror("minishell: malloc"), false);
		*dst = ft_gethome(user);
		if (!*dst)
			return (perror("minishell: malloc"), free(user), false);
		free(user);
	}
	else if (!ft_strcmp(av[0], "-"))
	{
		*dst = ft_hmap_get(&ctx->env, "OLDPWD");
		if (!*dst)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n",
					STDERR_FILENO), false);
		*dst = ft_strdup(*dst);
		if (!*dst)
			return (perror("minishell: malloc"), false);
	}
	return (true);
}

static inline int32_t	_print_chdir_err(char *dir, char *dir_f, char *old_cwd)
{
	char	*err;
	int32_t	errno_;

	errno_ = errno;
	err = ft_calloc(16 + ft_strlen(dir), sizeof(char));
	if (!err)
		return (perror("minishell: malloc"), free(dir_f), free(old_cwd), 1);
	ft_strlcat(err, "minishell: cd: ", 16 + ft_strlen(dir));
	ft_strlcat(err, dir, 16 + ft_strlen(dir));
	errno = errno_;
	return (perror(err), free(err), free(dir_f), free(old_cwd), 1);
}

static inline int32_t	_print_cwd(bool should_print, char *cwd)
{
	if (should_print)
	{
		ft_putstr_fd(cwd, STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	return (0);
}

int32_t	main_cd(int32_t ac, char **av, t_sh_ctx *ctx)
{
	char	*dir;
	char	*dir_f;
	char	*old_cwd;
	char	*new_cwd;

	if (_check_invalid_opt(&ac, &av))
		return (2);
	if (ac > 1)
		return (ft_putstr_fd("minishell: cd: too many arguments\n",
				STDERR_FILENO), 1);
	if (!_get_dest_dir(&dir, ac, av, ctx))
		return (1);
	dir_f = (char *)((uintptr_t)dir * (!ac || !ft_strcmp(av[0], "-")));
	old_cwd = getcwd(NULL, 0);
	if (!old_cwd)
		return (perror("minishell: malloc"), free(dir_f), 1);
	if (chdir(dir))
		return (_print_chdir_err(dir, dir_f, old_cwd));
	env_set(&ctx->env, "OLDPWD", old_cwd);
	new_cwd = getcwd(NULL, 0);
	if (!new_cwd)
		return (perror("minishell: malloc"), free(dir_f), 1);
	env_set(&ctx->env, "PWD", new_cwd);
	return (free(dir_f), _print_cwd(ac > 0 && !ft_strcmp(av[0], "-"), new_cwd));
}
