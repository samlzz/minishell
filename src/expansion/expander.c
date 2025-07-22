/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:21:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/22 15:14:36 by sliziard         ###   ########.fr       */
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
