/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:21:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/23 23:02:23 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <stdlib.h>

static bool	_is_export_valid_key(t_token *cur)
{
	size_t	i;
	bool	resp;
	char	*key;

	key = NULL;
	while (cur && cur->type != TK_ASSIGN)
	{
		if (ft_strjreplace(&key, cur->value))
			return (free(key), false);
		cur = cur->next;
		if (cur && !cur->glued && ft_strjreplace(&key, " "))
			return (free(key), false);
	}
	if (!key || (*key >= '0' && *key <= '9'))
		return (free(key), false);
	i = 0;
	while (key[i] && (ft_isalnum(key[i]) || key[i] == '_'))
		i++;
	if (!i)
		resp = false;
	else
	resp = key[i] == '\0';
	free(key);
	return (resp);
}

static inline int16_t	_handle_assign_tk(t_token **cur, t_argword *args,
											t_argword **append, bool val_split)
{
	t_argword	*last;

	if (*cur && (*cur)->type == TK_ASSIGN)
	{
		last = argword_getlast(args);
		if (ft_strjreplace(&last->value, "="))
			return (-1);
		next(cur);
		if (*cur && (*cur)->glued)
			*append = last;
		return (val_split);
	}
	return (1);
}

static inline int16_t	_append_value_and_lst(t_argword **lst,
										t_argword *target, t_argword **entry)
{
	t_argword	*to_del;

	if (!target)
	{
		argword_add_back(lst, *entry);
		return (0);
	}
	if (!argword_append_value(target, (*entry)->value, QUOTE_SINGLE))
		return (1);
	to_del = *entry;
	*entry = (*entry)->next;
	to_del->next = NULL;
	argword_clear(to_del);
	if (*entry)
		argword_add_back(lst, *entry);
	return (0);
}

t_argword	*expand_export_cmd(t_token *cur, t_sh_ctx *ctx)
{
	t_argword	*args;
	t_argword	*entry;
	int16_t		split;
	bool		val_split;
	t_argword	*append;

	args = NULL;
	append = NULL;
	split = true;
	expand_tild_export(cur, ctx->env);
	while (cur)
	{
		val_split = !_is_export_valid_key(cur);
		entry = expand_word(ctx, &cur, split, true);
		if (!entry)
			return (argword_clear(args), NULL);
		if (_append_value_and_lst(&args, append, &entry))
			return (argword_clear(args), argword_clear(entry), NULL);
		append = NULL;
		split = _handle_assign_tk(&cur, args, &append, val_split);
		if (split < 0)
			return (argword_clear(args), NULL);
	}
	return (args);
}

bool	is_export_cmd(t_token *argv)
{
	char	*argv0;
	bool	resp;

	argv0 = NULL;
	while (argv && argv->value)
	{
		ft_strjreplace(&argv0, argv->value);
		argv = argv->next;
		if (argv && !argv->glued)
			break ;
	}
	resp = ft_strcmp(argv0, "export") == 0;
	free(argv0);
	return (resp);
}
