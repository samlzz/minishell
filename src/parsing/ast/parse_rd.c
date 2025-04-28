/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:22:39 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/28 12:43:09 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include <stdlib.h>

static inline t_redir_type	_get_rd_type(t_tk_type tk)
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
	t_token			*fname;
	t_redir_type	type;
	t_ast			*redir;

	type = _get_rd_type((*cur)->type);
	next(cur);
	if (!*cur || (*cur)->type != TK_WORD || !*(*cur)->value)
		return ((*errtok = *cur), NULL);
	fname = *cur;
	if (fname->unexpanded && fname->next && fname->next->unexpanded && \
		ft_strcmp(fname->unexpanded, fname->next->unexpanded) == 0)
		return ((*errtok = fname), NULL);
	next(cur);
	redir = ft_calloc(1, sizeof(t_ast));
	if (!redir)
		return (NULL);
	redir->type = ND_REDIR;
	redir->u_data.rd.redir_type = type;
	if (type == RD_HEREDOC && fname->quote == QUOTE_NONE)
		redir->u_data.rd.hd_expand = true;
	redir->u_data.rd.filename = ft_strdup(fname->value);
	if (!redir->u_data.rd.filename)
		return (ast_free(redir), NULL);
	return (redir);
}

static t_ast	*_collect_redirs(t_token **cur, t_ast *inner, t_token **errtok)
{
	t_ast	*redir;

	while (*cur && is_redirection((*cur)->type))
	{
		redir = _parse_single_redir(cur, errtok);
		if (!redir)
		{
			ast_free(inner);
			return (NULL);
		}
		redir->u_data.rd.child = inner;
		inner = redir;
	}
	return (inner);
}

static t_ast	*_collect_command(t_token **cur, t_ast *expr, t_token **errtok)
{
	char	**new;
	char	**tmp;

	if (!expr)
		return (primary_parser(cur, errtok));
	if (!*cur || (*cur)->type != TK_WORD || \
		expr->type != ND_CMD || !expr->u_data.s_cmd.argv)
		return ((*errtok = *cur), ast_free(expr), NULL);
	new = collect_argv(cur, errtok);
	if (!new)
		return (ast_free(expr), NULL);
	tmp = join_argv(expr->u_data.s_cmd.argv, new);
	if (!tmp)
		return (ft_splitfree(new, 0), ast_free(expr), NULL);
	free(new);
	free(expr->u_data.s_cmd.argv);
	expr->u_data.s_cmd.argv = tmp;
	return (expr);
}

t_ast	*redir_parser(t_token **cur, t_token **errtok)
{
	t_ast	*rd_list;
	t_ast	*expr;
	t_ast	*last;

	rd_list = NULL;
	expr = NULL;
	while (*cur && ((*cur)->type == TK_WORD \
		|| (*cur)->type == TK_LPAREN \
		|| is_redirection((*cur)->type)))
	{
		if ((*cur)->type == TK_WORD || (*cur)->type == TK_LPAREN)
		{
			expr = _collect_command(cur, expr, errtok);
			if (!expr)
				return (ast_free(rd_list), NULL);
		}
		else
		{
			rd_list = _collect_redirs(cur, rd_list, errtok);
			if (!rd_list)
				return (ast_free(expr), NULL);
		}
	}
	if (!rd_list)
		return (expr);
	last = rd_list;
	while (last->type == ND_REDIR && last->u_data.rd.child)
		last = last->u_data.rd.child;
	last->u_data.rd.child = expr;
	return (rd_list);
}
