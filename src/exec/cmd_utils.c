/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 22:15:52 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/30 02:32:58 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "exec/exec.h"

static bool	_test_paths(char **paths, char *cmd, char **ret)
{
	char	*cmd_;
	char	*tmp;
	size_t	i;

	tmp = NULL;
	i = -1;
	while (paths[++i])
	{
		if (ft_strlen(paths[i]))
			cmd_ = ft_str3join(paths[i], "/", cmd);
		else
			cmd_ = ft_str3join(".", "/", cmd);
		if (!cmd_)
			return (perror("minishell: malloc"), free(tmp), *ret = NULL, false);
		if (!access(cmd_, X_OK))
			return (free(tmp), *ret = cmd_, true);
		else if (!access(cmd_, F_OK))
		{
			free(tmp);
			tmp = cmd_;
		}
		else
			free(cmd_);
	}
	return (*ret = tmp, true);
}

char	*exec_get_cmd_path(char **av, t_sh_ctx *ctx)
{
	char	**paths;
	char	*path_str;
	char	*cmd;

	if (!av[0])
		cmd = ft_strdup("");
	if (av[0] && ft_strchr(av[0], '/'))
		cmd = ft_strdup(av[0]);
	if ((!av[0] || ft_strchr(av[0], '/')) && !cmd)
		perror("minishell: malloc");
	if (!av[0] || ft_strchr(av[0], '/'))
		return (cmd);
	path_str = ft_hmap_get(&ctx->env, "PATH");
	if (!path_str && ctx->use_fallback_path)
		path_str = ENV_DEFAULT_PATH;
	else if (!path_str)
		path_str = "";
	paths = ft_nsplit(path_str, ':');
	if (!paths)
		return (perror("minishell: malloc"), NULL);
	if (!_test_paths(paths, av[0], &cmd))
		return (free(paths), NULL);
	else if (!cmd)
		cmd = ft_strdup(av[0]);
	if (!cmd)
		 perror("minishell: malloc");
	return (free(paths), cmd);
}
