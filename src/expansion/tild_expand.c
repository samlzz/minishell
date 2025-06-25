/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tild_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:23:30 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/26 00:02:32 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <stdlib.h>

static bool	_is_tilde_expandable(t_token *tok)
{
	if (!tok || !tok->value)
		return (false);
	if (tok->value[0] != '~')
		return (false);
	if (tok->quote != QUOTE_NONE)
		return (false);
	if (tok->glued || (tok->next && \
		(tok->next->glued && tok->next->quote != QUOTE_NONE)))
		return (false);
	return (true);
}

static inline char	*_expand_alone_tild(t_hmap *env, char *username)
{
	char	*user;
	char	*home;
	
	home = ft_hmap_get(env, "HOME");
	if (home)
		return (ft_strdup(home));
	if (!username)
	{
		user = ft_getuser();
		home = ft_gethome(user);
		free(user);
	}
	else
		home = ft_gethome(username);
	return (home);
}

static inline char	*_expand_parametered_tild(char *tk_val, t_hmap *env)
{
	char	*home;
	char	*tmp;
	char	*res;
	size_t	len;
	size_t	i;

	i = 0;
	while (tk_val[i] && tk_val[i] != '/')
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

void	expand_tild(t_token *cur, t_sh_ctx *ctx)
{
	char	*tmp;

	while (cur)
	{
		if (_is_tilde_expandable(cur))
		{
			if (cur->value[1] == '\0')
				tmp = _expand_alone_tild(&ctx->env, NULL);
			else
				tmp = _expand_parametered_tild(cur->value, &ctx->env);
			if (tmp)
			{
				free(cur->value);
				cur->value = tmp;
				cur->quote = QUOTE_SINGLE;
			}
		}
		cur = cur->next;
	}
}
