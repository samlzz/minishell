/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:21:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/23 20:57:32 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <stdlib.h>

t_argword	*expand_word(t_sh_ctx *ctx, t_token **cur, bool split,
															bool assign_stop)
{
	t_argword	*splitted;
	t_argword	*expanded;

	expanded = fill_argword(ctx, cur, assign_stop);
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
	expand_tild(cur, ctx->env);
	file = expand_word(ctx, &cur, false, false);
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

int16_t	expand_command(t_ast *cmd, t_sh_ctx *ctx)
{
	t_argword	*args;
	t_argword	*field;
	t_token		*cur;

	args = NULL;
	cur = cmd->u_data.cmd.args->tk;
	if (is_export_cmd(cur))
		args = expand_export_cmd(cur, ctx);
	else
	{
		while (cur)
		{
			expand_tild(cur, ctx->env);
			field = expand_word(ctx, &cur, true, false);
			if (!field)
				return (argword_clear(args), 1);
			argword_add_back(&args, field);
		}
	}
	if (!args)
		return (1);
	return (token_clear(cmd->u_data.cmd.args->tk), free(cmd->u_data.cmd.args),
		cmd->u_data.cmd.args = NULL, cmd->u_data.cmd.is_expanded = true,
		_construct_argv(&cmd->u_data.cmd.args, args),
		argword_clear(args), !cmd->u_data.cmd.args);
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
