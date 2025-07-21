/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:21:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/21 22:13:56 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <stdlib.h>

t_argword	*expand_word(t_sh_ctx *ctx, t_token **cur, bool split)
{
	t_argword	*splitted;
	t_argword	*expanded;

	expand_tild(*cur, ctx->env);
	expanded = fill_argword(ctx, cur);
	if (!expanded)
		return (NULL);
	if (split && expanded->space_offsets.len)
	{
		splitted = split_withespaces(expanded);
		argword_clear(expanded);
		if (!splitted)
			return (NULL);
		expanded = splitted;
	}
	return (replace_wildcards(expanded));
}

int16_t	expand_redir(t_ast *rd, t_sh_ctx *ctx, t_token **errtok)
{
	t_argword	*file;
	t_token		*cur;

	cur = rd->u_data.rd.filename.tk;
	file = expand_word(ctx, &cur, false);
	if (!file)
		return (1);
	if (argword_size(file) > 1)
	{
		*errtok = rd->u_data.rd.filename.tk;
		argword_clear(file);
		return (1);
	}
	token_clear(rd->u_data.rd.filename.tk);
	rd->u_data.rd.filename.expanded = file->value;
	rd->u_data.rd.is_expanded = true;
	file->value = NULL;
	argword_clear(file);
	return (0);
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

static inline char	*_get_export_key(t_token *cur)
{
	char	*tmp;
	char	*val;
	char	*eq;

	val = NULL;
	while (cur)
	{
		tmp = ft_strappend(val, cur->value);
		free(val);
		val = tmp;
		cur = cur->next;
		if (cur && !cur->glued)
		{
			tmp = ft_strappend(val, " ");
			free(val);
			val = tmp;
		}
		if (!val)
			return (NULL);
	}
	eq = ft_strchr(val, '=');
	if (eq)
		*eq = 0;
	return (val);
}

static bool	_is_export_valid_key(t_token *cur)
{
	size_t	i;
	bool	resp;
	char	*key;

	key = _get_export_key(cur);
	if (!key)
		return (false);
	if (*key >= '0' && *key <= '9')
		return (false);
	i = 0;
	while (key[i] && (ft_isalnum(key[i]) || key[i] == '_'))
		i++;
	resp = key[i] == '\0';
	free(key);
	return (resp);
}

int16_t	expand_export_args(t_ast *cmd, t_argword **args, t_sh_ctx *ctx)
{
	t_argword	*entry;
	t_token		*head;
	t_token 	*cur;
	bool		split;

	cur = cmd->u_data.cmd.args->tk;
	head = cur;
	split = true;
	expand_tild_export(cur, ctx->env);
	while (cur)
	{
		entry = expand_word(ctx, &cur, split);
		if (!entry)
			return (argword_clear(*args), 1);
		argword_add_back(args, entry);
		if (entry->value && ft_strchr(entry->value, '='))
			split = !_is_export_valid_key(head);
	}
	return (0);
}

int16_t	expand_command(t_ast *cmd, t_sh_ctx *ctx)
{
	t_argword	*args;
	t_argword	*field;
	t_token		*cur;

	args = NULL;
	cur = cmd->u_data.cmd.args->tk;
	while (cur)
	{
		field = expand_word(ctx, &cur, true);
		if (!field)
			return (argword_clear(args), 1);
		argword_add_back(&args, field);
		if (argword_size(args) == 1 && field->value && !ft_strcmp(field->value, "export"))
		{
			if (expand_export_args(cmd, &args, ctx))
				return (1);
			else
				break ;
		}
	}
	token_clear(cmd->u_data.cmd.args->tk);
	free(cmd->u_data.cmd.args);
	cmd->u_data.cmd.args = NULL;
	cmd->u_data.cmd.is_expanded = true;
	_construct_argv(&cmd->u_data.cmd.args, args);
	argword_clear(args);
	return (!cmd->u_data.cmd.args);
}

int16_t	expand_node(t_sh_ctx *ctx, t_ast *node, t_token **errtok)
{
	if (node->type == ND_CMD)
	{
		if (!node->u_data.cmd.args || !node->u_data.cmd.args->tk)
			return (1);
		return (expand_command(node, ctx));
	}
	if (node->type == ND_REDIR)
	{
		if (node->u_data.rd.redir_type == RD_HEREDOC)
			return (0);
		if (!node->u_data.rd.filename.tk)
			return (1);
		return (expand_redir(node, ctx, errtok));
	}
	return (1);
}
