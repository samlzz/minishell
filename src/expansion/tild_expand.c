/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tild_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:23:30 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/23 17:17:15 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <stdlib.h>

static char	*_expand_alone_tild(t_env *env, char *username)
{
	char	*user;
	char	*home;

	if (username)
		return (ft_gethome(username));
	home = env_get(env, "HOME");
	if (home)
		return (ft_strdup(home));
	user = ft_getuser();
	home = ft_gethome(user);
	free(user);
	return (home);
}

static char	*_expand_parametered_tild(char *tk_val, t_env *env)
{
	char	*home;
	char	*tmp;
	char	*res;
	size_t	len;
	size_t	i;

	i = 0;
	while (tk_val[i] && !ft_strchr("/:", tk_val[i]))
		i++;
	if (i <= 1)
		tmp = NULL;
	else
		tmp = ft_substr(tk_val, 1, i - 1);
	home = _expand_alone_tild(env, tmp);
	free(tmp);
	len = ft_strlen(tk_val);
	if (len == i || !home)
		return (home);
	tmp = ft_substr(tk_val, i, len);
	if (!tmp)
		return (free(home), NULL);
	res = ft_strjoin(home, tmp);
	free(tmp);
	free(home);
	return (res);
}

static inline char	*_expand_arg(char *arg, t_env *env)
{
	char	*tmp;

	if (*arg != '~')
		return (NULL);
	if (arg[1] == '\0')
		tmp = _expand_alone_tild(env, NULL);
	else
		tmp = _expand_parametered_tild(arg, env);
	return (tmp);
}

void	expand_tild_export(t_token *argv, t_env *env)
{
	char	*tmp;

	while (argv)
	{
		while (argv && argv->type != TK_ASSIGN)
			argv = argv->next;
		if (argv)
			argv = argv->next;
		tmp = NULL;
		if (argv && argv->value && argv->quote == QUOTE_NONE \
			&& (!argv->next || !argv->next->glued \
				|| argv->next->quote == QUOTE_NONE \
				|| ft_strchr(argv->value, '/') || ft_strchr(argv->value, ':'))
		)
		{
			tmp = _expand_arg(argv->value, env);
		}
		if (tmp)
		{
			free(argv->value);
			argv->value = tmp;
		}
		if (argv)
			argv = argv->next;
	}
}

void	expand_tild(t_token *cur, t_env *env)
{
	char	*tmp;

	while (cur)
	{
		tmp = NULL;
		if (cur && cur->value && *cur->value == '~' \
			&& cur->quote == QUOTE_NONE && !cur->glued \
			&& (!cur->next || !cur->next->glued \
				|| cur->next->quote == QUOTE_NONE \
				|| ft_strchr(cur->value, '/'))
		)
		{
			tmp = _expand_arg(cur->value, env);
		}
		if (tmp)
		{
			free(cur->value);
			cur->value = tmp;
			cur->quote = QUOTE_SINGLE;
		}
		cur = cur->next;
	}
}
