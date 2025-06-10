/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:22:39 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/10 10:17:26 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast/ast.h"
#include "utils/utils.h"	
#include <stdlib.h>

/**
 * @brief Parse a single redirection from the token stream.
 * 
 * Validates filename and builds a ND_REDIR AST node.
 * 
 * @param cur Pointer to the token cursor.
 * @param errtok Output pointer to the token causing error.
 * @return t_ast* The redirection AST node or NULL.
 */
static t_ast	*_parse_single_redir(t_token **cur, t_token **errtok)
{
	t_token			*new;
	t_redir_type	type;
	t_ast			*redir;

	type = get_rd_type((*cur)->type);
	next(cur);
	if (!*cur || (*cur)->type != TK_WORD || !*(*cur)->value)
		return ((*errtok = *cur), NULL);
	redir = ft_calloc(1, sizeof(t_ast));
	if (!redir)
		return (NULL);
	redir->type = ND_REDIR;
	redir->u_data.rd.redir_type = type;
	while (*cur)
	{
		new = token_dup(*cur);
		if (!new)
			return (ast_free(redir, false), NULL);
		token_addback(&redir->u_data.rd.filename.tk, *cur);
		next(cur);
		if (!(*cur)->glued)
			break;
	}
	return (redir);
}

/**
 * @brief Create NC_CMD node or append TK_WORD tokens to it.
 * 
 * Add args when redirections appear before the end of the command.
 * 
 * @param cur Token cursor.
 * @param expr A ND_CMD AST node.
 * @param errtok Error token output.
 * @return t_ast* Updated command node or NULL on error.
 */
static t_ast	*_collect_command(t_token **cur, t_ast *expr, t_token **errtok)
{
	t_words	*new;
	t_token	*last;

	if (!expr)
		return (primary_parser(cur, errtok));
	if ((*cur)->type != TK_WORD || expr->type != ND_CMD)
	{
		*errtok = *cur;
		ast_free(expr, false);
		return (NULL);
	}
	new = collect_args(cur, errtok);
	if (!new)
		return (ast_free(expr, false), NULL);
	last = expr->u_data.cmd.args->tk;
	while (last && last->next)
		last = last->next;
	last->next = new->tk;
	free(new);
	return (expr);
}

static void	_rd_add_last(t_ast **subtree, t_ast *leaf)
{
	t_ast	*last;

	if (!*subtree)
	{
		*subtree = leaf;
		return ;
	}
	last = *subtree;
	while (last && last->type == ND_REDIR && last->u_data.rd.child)
		last = last->u_data.rd.child;
	last->u_data.rd.child = leaf;
}

t_ast	*redir_parser(t_token **cur, t_token **errtok)
{
	t_ast	*rd_subtree;
	t_ast	*expr;
	t_ast	*new;

	rd_subtree = NULL;
	expr = NULL;
	while (*cur && ((*cur)->type == TK_WORD \
		|| (*cur)->type == TK_LPAREN \
		|| is_redirection((*cur)->type)))
	{
		if ((*cur)->type == TK_WORD || (*cur)->type == TK_LPAREN)
			new = _collect_command(cur, expr, errtok);
		else
			new = _parse_single_redir(cur, errtok);
		if (!new)
			return (ast_free(rd_subtree, false), NULL);
		if (new->type == ND_REDIR)
			_rd_add_last(&rd_subtree, new);
		else
			expr = new;
	}
	if (expr)
		_rd_add_last(&rd_subtree, expr);
	return (rd_subtree);
}
