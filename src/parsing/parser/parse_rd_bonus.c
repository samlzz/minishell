/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rd_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 20:22:39 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 10:28:46 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "ast/ast.h"
#include "parser/parser.h"

#ifdef MINISHELL_BONUS

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
	if (((*cur)->type != TK_WORD && (*cur)->type != TK_ASSIGN) \
		|| expr->type != ND_CMD)
	{
		*errtok = *cur;
		return (NULL);
	}
	new = collect_args(cur, errtok);
	if (!new)
		return (NULL);
	last = expr->u_data.cmd.args->tk;
	while (last && last->next)
		last = last->next;
	last->next = new->tk;
	free(new);
	return (expr);
}

t_ast	*redir_parser(t_token **cur, t_token **errtok)
{
	t_ast	*rd_subtree;
	t_ast	*expr;
	t_ast	*new;

	rd_subtree = NULL;
	expr = NULL;
	while (*cur && ((*cur)->type == TK_WORD || (*cur)->type == TK_LPAREN \
		|| (*cur)->type == TK_ASSIGN || is_redirection((*cur)->type)))
	{
		if ((*cur)->type == TK_WORD || (*cur)->type == TK_LPAREN)
			new = _collect_command(cur, expr, errtok);
		else
			new = parse_single_redir(cur, errtok);
		if (!new)
			return (ast_free(rd_subtree), ast_free(expr), NULL);
		if (new->type == ND_REDIR)
			rd_add_last(&rd_subtree, new);
		else
			expr = new;
	}
	if (expr)
		rd_add_last(&rd_subtree, expr);
	if (!rd_subtree)
		*errtok = *cur;
	return (rd_subtree);
}

#endif