/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/08/01 12:11:21 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "libft.h"
#include "builtins.h"

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
	*dst = av[0];
	if (ac == 0)
	{
		*dst = env_get(ctx->env, "HOME");
		if (!*dst)
			return (ft_putstr_fd("minishell: cd: HOME not set\n",
					STDERR_FILENO), false);
	}
	else if (!ft_strcmp(av[0], "-"))
	{
		*dst = env_get(ctx->env, "OLDPWD");
		if (!*dst)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n",
					STDERR_FILENO), false);
	}
	*dst = ft_strdup(*dst);
	if (!*dst)
		return (perror("minishell: malloc"), false);
	return (true);
}

static inline int32_t	_print_chdir_err(char *dir)
{
	int32_t	errno_;

	errno_ = errno;
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd(dir, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno_), STDERR_FILENO);
	free(dir);
	return (1);
}

static inline int32_t	_print_and_update_pwd(bool should_print, t_sh_ctx *ctx)
{
	char	*pwd;
	char	*tmp;
	bool	ok;

	pwd = env_get(ctx->env, "PWD");
	if (!pwd)
		tmp = ft_strdup("OLDPWD");
	else
		tmp = ft_strjoin("OLDPWD=", pwd);
	if (!tmp || env_set(ctx->env, tmp))
		return (perror("minishell: malloc"), free(tmp), 1);
	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (perror("cd: error retrieving current directory: getcwd: " \
			"cannot access parent directories"), 1);
	tmp = ft_strjoin("PWD=", pwd);
	if (!tmp || env_set(ctx->env, tmp))
		return (perror("minishell: malloc"), free(tmp), free(pwd), 1);
	ok = true;
	if (should_print)
		ok = bi_putendl("cd", pwd);
	free(pwd);
	return (!ok);
}

int32_t	main_cd(int32_t ac, char **av, t_sh_ctx *ctx)
{
	char		*dir;
	char		*tmp;

	if (_check_invalid_opt(&ac, &av))
		return (2);
	if (ac > 1)
		return (ft_putstr_fd("minishell: cd: too many arguments\n",
				STDERR_FILENO), 1);
	if (!_get_dest_dir(&dir, ac, av, ctx))
		return (1);
	if (!dir[0])
		return (free(dir), _print_and_update_pwd(false, ctx));
	tmp = cd_absolute(ctx, dir);
	if (!tmp)
		return (perror("minishell: malloc"), free(dir), 1);
	tmp = cd_canonicalize(tmp);
	if ((!tmp || !tmp[0]) && errno == EACCES)
		return (errno = ENOTDIR, _print_chdir_err(dir));
	if (!tmp || !tmp[0])
		return (_print_chdir_err(dir));
	if (chdir(tmp))
		return (_print_chdir_err(dir), free(tmp), 1);
	free(tmp);
	return (free(dir), _print_and_update_pwd(ac > 0 && !ft_strcmp(av[0], "-"),
			ctx));
}
