/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tild_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 21:23:30 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/27 15:02:08 by sliziard         ###   ########.fr       */
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
	if (tok->glued || \
		(tok->next && tok->next->glued && tok->next->quote != QUOTE_NONE && \
		!ft_strchr(tok->value, '/')))
		return (false);
	return (true);
}

static char	*_expand_alone_tild(t_hmap *env, char *username)
{
	char	*user;
	char	*home;
	
	if (username)
		return (ft_gethome(username));
	home = ft_hmap_get(env, "HOME");
	if (home)
		return (ft_strdup(home));
	user = ft_getuser();
	home = ft_gethome(user);
	free(user);
	return (home);
}

static char	*_expand_parametered_tild(char *tk_val, t_hmap *env)
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

static inline char	*_expand_var_tild(t_token *var, t_hmap *env)
{
	char	*pref;
	char	*tmp;
	char	*eq;

	eq = ft_strchr(var->value, '=');
	if (!eq || eq[1] != '~')
		return (NULL);
	if (eq[2] == '\0')
		tmp = _expand_alone_tild(env, NULL);
	else
		tmp = _expand_parametered_tild(eq + 1, env);
	if (tmp)
	{
		pref = ft_substr(var->value, 0, (eq - var->value) + 1);
		if (pref && *pref)
			return (ft_strjoin(pref, tmp));
		else if (pref)
			free(pref);
	}
	return (tmp);
}

void	expand_tild(t_token *cur, t_sh_ctx *ctx)
{
	char	*tmp;

	while (cur)
	{
		tmp = NULL;
		if (_is_tilde_expandable(cur))
		{
			if (cur->value[1] == '\0')
				tmp = _expand_alone_tild(&ctx->env, NULL);
			else
				tmp = _expand_parametered_tild(cur->value, &ctx->env);
		}
		else if (!ft_strcmp(cur->value, "export"))
			tmp = _expand_var_tild(cur = cur->next, &ctx->env);
		if (tmp)
		{
			free(cur->value);
			cur->value = tmp;
			cur->quote = QUOTE_SINGLE;
		}
		cur = cur->next;
	}
}
