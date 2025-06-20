/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 22:15:52 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/16 22:16:40 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "exec/exec.h"

static inline char	*_concat_path_cmd(char *path, char *cmd)
{
	size_t	len;
	char	*ret;

	len = ft_strlen(path) + ft_strlen(cmd) + 2;
	ret = ft_calloc(len, sizeof(char));
	if (!ret)
		return (perror("exec: _concat_path_cmd: malloc"), NULL);
	ft_strlcat(ret, path, len);
	ft_strlcat(ret, "/", len);
	ft_strlcat(ret, cmd, len);
	return (ret);
}

static inline char	*_cmd_finder(char *cmd, char **path)
{
	size_t	i;
	char	*tmp;
	char	*f;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/') || ft_strlen(cmd) == 0)
		return (ft_strdup(cmd));
	f = NULL;
	i = -1;
	while (path[++i])
	{
		tmp = _concat_path_cmd(path[i], cmd);
		if (!tmp)
			return (free(f), NULL);
		if (!f && access(tmp, F_OK) != -1)
			f = tmp;
		if (access(tmp, X_OK) != -1)
			return (free((void *)((f != tmp) * (uintptr_t)(f))), tmp);
		else if (f != tmp)
			free(tmp);
	}
	if (f)
		return (f);
	return (free(f), ft_strdup(cmd));
}

char	*exec_get_cmd_path(char **av, t_hmap *envp)
{
	char	**path;
	char	*path_str;
	char	*cmd;


	path_str = ft_hmap_get(envp, "PATH");
	if (!path_str)
		return (ft_calloc(1, sizeof(char *)));
	path = ft_split(path_str, ':');
	if (!path)
		return (perror("exec: _get_cmd_path: ft_split"), NULL);
	cmd = _cmd_finder(av[0], path);
	ft_splitfree(path, 0);
	return (cmd);
}
