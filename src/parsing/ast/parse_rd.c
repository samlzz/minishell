/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:36:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/23 21:32:51 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

static inline bool	is_redirection(t_tk_type type)
{
	return (type == TK_REDIR_IN || type == TK_REDIR_OUT
		|| type == TK_REDIR_APPEND || type == TK_HEREDOC);
}

static inline t_redir_type	get_rd_type(t_tk_type tk)
{
	if (tk == TK_REDIR_IN)
		return RD_IN;
	if (tk == TK_REDIR_OUT)
		return RD_OUT;
	if (tk == TK_REDIR_APPEND)
		return RD_APPEND;
	if (tk == TK_HEREDOC)
		return RD_HEREDOC;
	return -1;
}

static t_ast	*_parse_single_redir(t_token **cur, t_token **errtok)
{
	t_token			*filename;
	t_redir_type	type;
	t_ast			*redir;

	type = get_rd_type((*cur)->type);
	next(cur);
	if (!*cur || (*cur)->type != TK_WORD)
		return ((*errtok = *cur), NULL);
	filename = *cur;
	next(cur);
	redir = ft_calloc(1, sizeof(t_ast));
	if (!redir)
		return (NULL);
	redir->type = ND_REDIR;
	redir->u_data.s_redir.redir_type = type;
	if (type == RD_HEREDOC && filename->quote == QUOTE_NONE)
		redir->u_data.s_redir.hd_expand = true;
	redir->u_data.s_redir.filename = ft_strdup(filename->value);
	if (!redir->u_data.s_redir.filename)
		return (ast_free(redir), NULL);
	return redir;
}

static t_ast	*_collect_redirs(t_token **cur, t_ast *inner, t_token **errtok)
{
	t_ast *redir;

	while (*cur && is_redirection((*cur)->type))
	{
		redir = _parse_single_redir(cur, errtok);
		if (!redir)
			return (ast_free(inner), NULL);
		redir->u_data.s_redir.child = inner;
		inner = redir;
	}
	return (inner);
}

static inline t_ast	*_check_words_left(t_ast *rd_tree, t_ast *expr, \
	t_token **cur, t_token **errtok)
{
	char	**new_args;
	char	**tmp;

	if (!*cur || (*cur)->type != TK_WORD)
		return (rd_tree);
	if ((*cur)->expand || expr->type != ND_CMD)
		return (*errtok = *cur, ast_free(rd_tree), NULL);
	new_args = collect_argv(cur, errtok);
	if (!new_args || !expr->u_data.s_cmd.argv)
		return (ast_free(rd_tree), NULL);
	tmp = join_argv(expr->u_data.s_cmd.argv, new_args);
	if (!tmp)
		return (ft_splitfree(new_args, 0), \
			ast_free(rd_tree), NULL);
	free(new_args);
	free(expr->u_data.s_cmd.argv);
	expr->u_data.s_cmd.argv = tmp;
	return (rd_tree);
}

t_ast	*redir_parser(t_token **cur, t_token **errtok)
{
	t_ast	*tree;
	t_ast	*expr;
	t_ast	*last;
	
	tree = _collect_redirs(cur, NULL, errtok);
	if (*errtok)
		return (NULL);
	expr = primary_parser(cur, errtok);
	if (!expr)
		return (ast_free(tree), NULL);
	if (tree)
	{
		last = tree;
		while (last->type == ND_REDIR && last->u_data.s_redir.child)
			last = last->u_data.s_redir.child;
		if (last->type != ND_REDIR)
			return (ast_free(tree), ast_free(expr), NULL);
		last->u_data.s_redir.child = expr;
	}
	else
		tree = expr;
	tree = _collect_redirs(cur, tree, errtok);
	if (!tree)
		return (ast_free(expr), NULL);
	return (_check_words_left(tree, expr, cur, errtok));
}
