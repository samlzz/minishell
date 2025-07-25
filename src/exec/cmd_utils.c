/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 22:15:52 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/25 10:36:19 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "exec.h"
#include "utils.h"
#include "env/env.h"

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

static bool	_check_cmd_path(char *name, char **cmd)
{
	if (!name || !name[0])
		*cmd = ft_strdup("");
	if (name && (ft_strchr(name, '/') || !ft_strcmp(name, ".")
			|| !ft_strcmp(name, "..")))
		*cmd = ft_strdup(name);
	if ((!name || !name[0] || ft_strchr(name, '/') || !ft_strcmp(name, ".")
			|| !ft_strcmp(name, "..")) && !*cmd)
		perror("minishell: malloc");
	if (!name || !name[0] || ft_strchr(name, '/') || !ft_strcmp(name, ".")
		|| !ft_strcmp(name, ".."))
		return (true);
	return (false);
}

char	*exec_get_cmd_path(char **av, t_sh_ctx *ctx)
{
	char	**paths;
	char	*path_str;
	char	*cmd;

	if (_check_cmd_path(av[0], &cmd))
		return (cmd);
	path_str = env_get(ctx->env, "PATH");
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

static inline void	_dup_err(int32_t oldfd, int32_t newfd)
{
	char	str[61];

	str[0] = 0;
	ft_strlcat(str, "minishell: cannot duplicate fd ", 61);
	ft_itoa_str(str + ft_strlen(str), oldfd);
	ft_strlcat(str, " to fd ", 61);
	ft_itoa_str(str + ft_strlen(str), newfd);
	perror(str);
}

void	ft_dup_fds(int32_t fds[2])
{
	if (dup2(fds[1], STDOUT_FILENO) == -1)
		_dup_err(fds[1], STDOUT_FILENO);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	if (dup2(fds[0], STDIN_FILENO) == -1)
		_dup_err(fds[0], STDIN_FILENO);
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
}
