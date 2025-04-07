/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:24:10 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/07 11:15:03 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

static char	**_collect_argv(t_token **cur, t_token **errtok)
{
	t_token	*tk;
	char	**argv;
	int32_t	size;

	tk = *cur;
	size = 0;
	while (tk && tk->type == TK_WORD)
	{
		tk = tk->next;
		size++;
	}
	if (!size)
		return (*errtok = tk, NULL);
	argv = ft_calloc(size + 1, sizeof (char *));
	if (!argv)
		return (NULL);
	size = 0;
	while (*cur && (*cur)->type == TK_WORD)
	{
		argv[size] = expand_and_join_words(cur, 0);
		if (!argv[size])
			return (ft_splitfree(argv, 0), NULL);
		size++;
	}
	return (argv);
}

static inline bool	_insert_cmd_leaf(t_ast *tree, t_ast *cmd)
{
	while (tree->type == ND_REDIR && tree->u_data.s_redir.child)
		tree = tree->u_data.s_redir.child;
	if (tree->type != ND_REDIR)
		return (false);
	tree->u_data.s_redir.child = cmd;
	return (true);
}

static bool	_handle_word(t_token **cur, t_ast **tree, \
	t_ast **cmd_node, t_token **errtok)
{
	if (*cmd_node)
		return (*errtok = *cur, false);
	*cmd_node = ft_calloc(1, sizeof (t_ast));
	if (!*cmd_node)
		return (false);
	(*cmd_node)->type = ND_CMD;
	(*cmd_node)->u_data.s_cmd.argv = _collect_argv(cur, errtok);
	if (!(*cmd_node)->u_data.s_cmd.argv)
		return (free(*cmd_node), *cmd_node = NULL, false);
	if (*tree)
		return (_insert_cmd_leaf(*tree, *cmd_node));
	*tree = *cmd_node;
	return (true);
}

static bool	_has_cmd_node(t_ast *tree)
{
	if (!tree)
		return (false);
	if (tree->type == ND_CMD)
		return (true);
	if (tree->type == ND_REDIR)
		return (_has_cmd_node(tree->u_data.s_redir.child));
	return (false);
}

t_ast	*cmd_parser(t_token **cur, t_token **errtok)
{
	t_ast	*tree;
	t_ast	*cmd_node;
	
	tree = NULL;
	cmd_node = NULL;
	while (*cur && ((*cur)->type == TK_WORD || _is_redirection((*cur)->type)))
	{
		if ((*cur)->type == TK_WORD)
		{
			if (!_handle_word(cur, &tree, &cmd_node, errtok))
				return (ast_free(tree), NULL);
		}
		else
		{
			if (!handle_redirection(cur, &tree, errtok))
				return (ast_free(tree), NULL);
		}
	}
	if (!_has_cmd_node(tree))
	{
		*errtok = *cur;
		ast_free(tree);
		return (NULL);
	}
	return (tree);
}
