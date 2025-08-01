/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:21:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/29 08:39:45 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdlib.h>

#include "expander.h"
#include "utils.h"

static bool	_is_export_valid_key(t_token *cur)
{
	size_t	i;
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
	if (key && key[0] != '\0' && key[0] != '=' && key[0] != '+' \
		&& !ft_isdigit(key[0]))
	{
		i = 0;
		while (ft_isalpha(key[i]) || ft_isdigit(key[i]) || key[i] == '_')
			i++;
		if (key[i] == '\0' || key[i] == '=' \
			|| (key[i] == '+' && key[i + 1] == '='))
			return (free(key), true);
	}
	return (free(key), false);
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
	if ((*entry)->value && !argword_append_value(target, (*entry)->value,
			QUOTE_SINGLE))
		return (1);
	to_del = *entry;
	*entry = (*entry)->next;
	to_del->next = NULL;
	argword_clear(to_del);
	if (*entry)
		argword_add_back(lst, *entry);
	return (0);
}

static inline int16_t	_handle_assign_tk(t_token **cur, t_argword **args,
											t_argword **append, bool val_split)
{
	t_argword	*last;

	if (*cur && (*cur)->type == TK_ASSIGN)
	{
		if ((*cur)->glued)
		{
			last = argword_getlast(*args);
			if (ft_strjreplace(&last->value, "="))
				return (-1);
		}
		else
		{
			last = argword_new();
			if (!last)
				return (-1);
			last->value = (*cur)->value;
			(*cur)->value = NULL;
			argword_add_back(args, last);
		}
		next(cur);
		if (*cur && (*cur)->glued)
			*append = last;
		return (val_split);
	}
	return (1);
}

#ifdef MINISHELL_BONUS

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
		entry = expand_word_handler(ctx, &cur, split, true);
		entry = export_wildcard_handler(args, entry, cur);
		if (!entry)
			return (argword_clear(args), NULL);
		if (_append_value_and_lst(&args, append, &entry))
			return (argword_clear(args), argword_clear(entry), NULL);
		append = NULL;
		split = _handle_assign_tk(&cur, &args, &append, val_split);
		if (split < 0)
			return (argword_clear(args), NULL);
	}
	return (args);
}

#else

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
		entry = expand_word_handler(ctx, &cur, split, true);
		if (!entry)
			return (argword_clear(args), NULL);
		if (_append_value_and_lst(&args, append, &entry))
			return (argword_clear(args), argword_clear(entry), NULL);
		append = NULL;
		split = _handle_assign_tk(&cur, &args, &append, val_split);
		if (split < 0)
			return (argword_clear(args), NULL);
	}
	return (args);
}

#endif
