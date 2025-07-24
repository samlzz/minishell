/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:30 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/24 10:55:42 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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

static inline int32_t	_print_chdir_err(char *dir, char *old_cwd)
{
	char	*err;
	int32_t	errno_;

	errno_ = errno;
	err = ft_calloc(16 + ft_strlen(dir), sizeof(char));
	if (!err)
		return (perror("minishell: malloc"), free(dir), free(old_cwd), 1);
	ft_strlcat(err, "minishell: cd: ", 16 + ft_strlen(dir));
	ft_strlcat(err, dir, 16 + ft_strlen(dir));
	errno = errno_;
	return (perror(err), free(err), free(dir), free(old_cwd), 1);
}

static inline int32_t	_print_and_update_pwd(bool should_print, t_sh_ctx *ctx,
																char *old_cwd)
{
	char	*new_cwd;
	char	*tmp;

	new_cwd = getcwd(NULL, 0);
	if (!new_cwd)
		return (perror("minishell: malloc"), 1);
	tmp = ft_strjoin("OLDPWD=", old_cwd);
	if (!tmp)
		return (perror("minishell: malloc"), free(old_cwd), free(new_cwd), 1);
	free(old_cwd);
	if (env_set(ctx->env, tmp))
		return (perror("minishell: malloc"), free(new_cwd), free(tmp), 1);
	tmp = ft_strjoin("PWD=", new_cwd);
	if (!tmp)
		return (perror("minishell: malloc"), free(new_cwd), 1);
	if (env_set(ctx->env, tmp))
		return (perror("minishell: malloc"), free(tmp), free(new_cwd), 1);
	if (should_print)
		ft_putendl_fd(new_cwd, STDOUT_FILENO);
	free(new_cwd);
	return (0);
}

int32_t	main_cd(int32_t ac, char **av, t_sh_ctx *ctx)
{
	char	*dir;
	char	*old;

	if (_check_invalid_opt(&ac, &av))
		return (2);
	if (ac > 1)
		return (ft_putstr_fd("minishell: cd: too many arguments\n",
				STDERR_FILENO), 1);
	if (!_get_dest_dir(&dir, ac, av, ctx))
		return (1);
	old = getcwd(NULL, 0);
	if (!old)
		return (_print_chdir_err(dir, NULL));
	if (!dir[0])
		return (free(dir), _print_and_update_pwd(false, ctx, old));
	if (chdir(dir))
		return (_print_chdir_err(dir, old));
	return (free(dir), _print_and_update_pwd(ac > 0 && !ft_strcmp(av[0], "-"),
			ctx, old));
}
