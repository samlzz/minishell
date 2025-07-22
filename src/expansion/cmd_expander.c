/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:21:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/22 16:05:16 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <stdlib.h>

static inline bool	_is_export_valid_key(const char *key)
{
	size_t	i;
	bool	resp;

	if (*key >= '0' && *key <= '9')
		return (false);
	i = 0;
	while (key[i] && (ft_isalnum(key[i]) || key[i] == '_'))
		i++;
	resp = key[i] == '\0';
	return (resp);
}

static t_argword	*_expand_export_cmd(t_token *cur, t_sh_ctx *ctx)
{
	t_argword 	*args;
	t_argword	*entry;
	bool		split;
	t_argword	*append;

	args = NULL;
	split = true;
	expand_tild_export(cur, ctx->env);
	append = NULL;
	while (cur)
	{
		entry = expand_word(ctx, &cur, split, true);
		if (!entry)
			return (argword_clear(args), NULL);
		if (append)
		{
			if (!argword_append_value(append, entry->value, QUOTE_SINGLE))
				return (argword_clear(args), NULL);
			argword_clear(entry);
		}
		else
			argword_add_back(&args, entry);
		append = NULL;
		if (cur && cur->type == TK_ASSIGN)
		{
			entry = argword_getlast(entry);
			split = !_is_export_valid_key(entry->value);
			if (ft_strjreplace(&entry->value, "="))
				return (argword_clear(args), NULL);
			cur = cur->next;
			if (cur && cur->glued)
				append = entry;
		}
	}
	return (args);
}

static inline bool	_is_export(t_token *argv)
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

static inline void	_construct_argv(t_words **dest, t_argword *args)
{
	size_t		i;
	size_t		size;

	size = argword_size(args);
	if (!size)
		return ;
	*dest = ft_calloc(size + 1, sizeof(t_words));
	if (!*dest)
		return ;
	i = 0;
	while (args)
	{
		if (!args->is_expanded || (args->value && *args->value))
		{
			(*dest)[i++].expanded = args->value;
			args->value = NULL;
		}
		args = args->next;
	}
}

int16_t	expand_command(t_ast *cmd, t_sh_ctx *ctx)
{
	t_argword	*args;
	t_argword	*field;
	t_token		*cur;

	args = NULL;
	cur = cmd->u_data.cmd.args->tk;
	if (_is_export(cur))
		args = _expand_export_cmd(cur, ctx);
	else
		while (cur)
		{
			expand_tild(cur, ctx->env);
			field = expand_word(ctx, &cur, true, false);
			if (!field)
				return (argword_clear(args), 1);
			argword_add_back(&args, field);
		}
	if (!args)
		return (1);
	token_clear(cmd->u_data.cmd.args->tk);
	free(cmd->u_data.cmd.args);
	cmd->u_data.cmd.args = NULL;
	cmd->u_data.cmd.is_expanded = true;
	_construct_argv(&cmd->u_data.cmd.args, args);
	return (argword_clear(args), !cmd->u_data.cmd.args);
}
