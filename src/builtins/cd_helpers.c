/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 10:30:56 by mle-flem          #+#    #+#             */
/*   Updated: 2025/08/01 04:25:17 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/stat.h>

#include "env/env.h"
#include "libft.h"
#include "utils.h"

char	*cd_absolute(t_sh_ctx *ctx, char *path)
{
	char	*pwd;

	pwd = env_get(ctx->env, "PWD");
	if (!pwd)
		pwd = "";
	if (path && path[0] == '/')
		return (ft_strdup(path));
	else if ((!path || !path[0]) && pwd[ft_strlen(pwd) - 1] != '/')
		return (ft_strjoin(pwd, "/."));
	else if (!path || !path[0])
		return (ft_strjoin(pwd, "."));
	else if (pwd[ft_strlen(pwd) - 1] != '/')
		return (ft_str3join(pwd, "/", path));
	return (ft_strjoin(pwd, path));
}

static bool	_is_dir(char *path, char *cursor)
{
	struct stat	sb;
	char		c;
	bool		ret;

	c = *cursor;
	*cursor = 0;
	ret = (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode));
	*cursor = c;
	return (ret);
}

static inline bool	_canonicalize_next_dir(char *path, char *base, char **p,
																char **cursor)
{
	if ((*p)[0] == '/')
		(*p)++;
	else if ((*p)[0] == '.' && ((*p)[1] == '/' || !(*p)[1]))
		(*p)++;
	else if ((*p)[0] == '.' && (*p)[1] == '.' && ((*p)[2] == '/' || !(*p)[2]))
	{
		(*p) += 2;
		if (*cursor > base && !_is_dir(path, *cursor))
			return (false);
		while (--(*cursor) > base && (*cursor)[0] != '/')
			;
	}
	else
	{
		if (*cursor != base)
			*(*cursor)++ = '/';
		while ((*p)[0] && (*p)[0] != '/')
			*(*cursor)++ = *(*p)++;
		if (!_is_dir(path, *cursor))
			return (false);
	}
	return (true);
}

char	*cd_canonicalize(char *path)
{
	char	*base;
	char	*p;
	char	*cursor;
	bool	dslash_path;

	base = path + 1;
	dslash_path = ((path[0] == '/') && (path[1] == '/') && path[2] != '/');
	base += dslash_path;
	p = base;
	cursor = base;
	while (*p)
		if (!_canonicalize_next_dir(path, base, &p, &cursor))
			return (free(path), NULL);
	if (cursor == path)
		*cursor++ = '/';
	*cursor = 0;
	if (path[0] == '/' && path[1] == '/' && path[2] != '/' && !dslash_path)
	{
		if (!path[2])
			path[1] = 0;
		else
			ft_memmove(path, path + 1, ft_strlen(path + 1) + 1);
	}
	return (path);
}
