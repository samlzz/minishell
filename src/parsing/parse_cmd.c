/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:17:51 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/03 11:30:26 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

static inline bool _is_redirection(t_tk_type type)
{
	return (type == TK_REDIR_IN || type == TK_REDIR_OUT || \
			type == TK_REDIR_APPEND || type == TK_HEREDOC);
}

static inline t_redir_type	_get_rd_type(t_tk_type tk_type)
{
	t_redir_type	type;
	
	type = -1;
	if (tk_type == TK_REDIR_IN)
		type = RD_IN;
	else if (tk_type == TK_REDIR_OUT)
		type = RD_OUT;
	else if (tk_type == TK_REDIR_APPEND)
		type = RD_APPEND;
	else if (tk_type == TK_HEREDOC)
		type = RD_HEREDOC;
	return (type);
}

static t_ast	*_parse_redirection(t_token **cur, t_ast *cmd, t_token **errtok)
{
	t_ast			*node;
	t_redir_type	type;
	t_token			*filename;

	if (!*cur || !_is_redirection((*cur)->type))
		return (*errtok = *cur, NULL);
	type = _get_rd_type((*cur)->type);
	*cur = (*cur)->next;
	if (!*cur || (*cur)->type != TK_WORD)
	{
		*errtok = *cur;
		return (NULL);
	}
	filename = *cur;
	*cur = (*cur)->next;
	node = ft_calloc(1, sizeof (t_ast));
	if (!node)
		return (NULL);
	node->type = ND_REDIR;
	node->u_data.s_redir.filename = ft_strdup(filename->value);
	node->u_data.s_redir.redir_type = type;
	node->u_data.s_redir.child = cmd;
	return (node);
}

static inline char	**_collect_argv(t_token **cur, t_token **errtok)
{
	t_token	*tk;
	char	**argv;
	int32_t	size;

	tk = *cur;
	size = 0;
	while (tk && tk->type == TK_WORD)
	{
		size++;
		tk = tk->next;
	}
	if (!size)
		return (*errtok = tk, NULL);
	argv = ft_calloc(size + 1, sizeof (char *));
	if (!argv)
		return (NULL);
	tk = *cur;
	size = 0;
	while (tk && tk->type == TK_WORD)
	{
		argv[size++] = ft_strdup(tk->value);
		tk = tk->next;
	}
	*cur = tk;
	return (argv);
}

t_ast	*cmd_parser(t_token **cur, t_token **errtok)
{
	t_ast	*cmd_node;
	t_ast	*rd_node;
	
	cmd_node = ft_calloc(1, sizeof (t_ast));
	if (!cmd_node)
		return (NULL);
	cmd_node->type = ND_CMD;
	cmd_node->u_data.s_cmd.argv = _collect_argv(cur, errtok);
	if (!cmd_node->u_data.s_cmd.argv)
	{
		free(cmd_node);
		return (NULL);
	}
	while (*cur && _is_redirection((*cur)->type))
	{
		rd_node = _parse_redirection(cur, cmd_node, errtok);
		if (!rd_node)
		{
			ast_free(cmd_node);
			return (NULL);
		}
		cmd_node = rd_node;
	}
	return (cmd_node);
}
