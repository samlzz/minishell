/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:21:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/10 16:49:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "heredoc/here_doc.h"
#include <stdlib.h>

t_argword	*expand_word(t_token **cur, bool split, t_hmap *env)
{
	t_argword	*splitted;
	t_argword	*expanded;

	expanded = fill_argword(env, cur, 0);
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

int16_t	expand_redir(t_ast *rd, t_hmap *env, t_token **errtok)
{
	t_argword	*file;
	t_token		*cur;

	cur = rd->u_data.rd.filename.tk;
	file = expand_word(&cur, false, env);
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
	while (i < size)
	{
		(*dest)[i++].expanded = args->value;
		args->value = NULL;
		args = args->next;
	}
}

int16_t	expand_command(t_ast *cmd, t_hmap *env)
{
	t_argword	*args;
	t_argword	*field;
	t_token		*cur;

	args = NULL;
	cur = cmd->u_data.cmd.args->tk;
	while (cur)
	{
		field = expand_word(&cur, true, env);
		if (!field)
			return (argword_clear(args), 1);
		argword_add_back(&args, field);
	}
	token_clear(cmd->u_data.cmd.args->tk);
	free(cmd->u_data.cmd.args);
	cmd->u_data.cmd.args = NULL;
	_construct_argv(&cmd->u_data.cmd.args, args);
	argword_clear(args);
	return (!cmd->u_data.cmd.args);
}

int16_t	expand_node(t_ast *node, t_hmap *env, t_token **errtok)
{
	if (node->type == ND_CMD)
	{
		if (!node->u_data.cmd.args || !node->u_data.cmd.args->tk)
			return (1);
		return (expand_command(node, env));
	}
	if (node->type == ND_REDIR)
	{
		if (!node->u_data.rd.filename.tk)
			return (1);
		if (node->u_data.rd.redir_type == RD_HEREDOC)
			return (expand_heredoc(node));
		return (expand_redir(node, env, errtok));
	}
	return (1);
}
