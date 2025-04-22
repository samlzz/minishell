/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 15:36:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/22 14:48:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"

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

static t_ast	*_build_rd_subtree(t_token **cur, t_ast *tree, t_token **errtok)
{
	t_ast			*new_rd;
	t_redir_type	type;
	t_token			*filename;

	while (*cur && is_redirection((*cur)->type))
	{
		type = get_rd_type((*cur)->type);
		next(cur);
		if (!*cur || (*cur)->type != TK_WORD)
			return (*errtok = *cur, ast_free(tree), NULL);
		filename = *cur;
		next(cur);
		new_rd = ft_calloc(1, sizeof (t_ast));
		if (!new_rd)
			return (ast_free(tree), NULL);
		new_rd->type = ND_REDIR;
		new_rd->u_data.s_redir.redir_type = type;
		new_rd->u_data.s_redir.filename = ft_strdup(filename->value);
		if (!new_rd->u_data.s_redir.filename)
			return (ast_free(tree), NULL);
		new_rd->u_data.s_redir.child = tree;
		tree = new_rd;
	}
	return (tree);
}

static inline bool	insert_expr_leaf(t_ast *tree, t_ast *leaf)
{
	while (tree->type == ND_REDIR && tree->u_data.s_redir.child)
		tree = tree->u_data.s_redir.child;
	if (tree->type != ND_REDIR)
		return (false);
	tree->u_data.s_redir.child = leaf;
	return (true);
}

/**
 * @brief Parses an expression surrounded by optional redirections.
 *
 * This function wraps a primary expression (either a command or a subshell)
 * with any redirections (`<`, `>`, `>>`, `<<`) that may precede or follow it.
 * The redirections are represented in the AST as ND_REDIR nodes stacked around
 * the core expression node. The parser supports both:
 *
 *   - leading redirections: `< infile (cmd)`
 * 
 *   - trailing redirections: `(cmd) > outfile`
 *
 * @param env      A pointer to the environment hashmap, used for expansions.
 * @param cur      A pointer to the current token pointer in the stream.
 * @param errtok   A pointer to the token pointer where a parsing error occurred, if any.
 *
 * @return A pointer to the resulting AST subtree, or NULL if a parsing error occurred.
 *
 * @note If an error is detected (e.g., missing filename after a redirection),
 *       the function sets `*errtok` to the offending token and frees any
 *       partially constructed AST.
 *
 * @see primary_parser
 */
t_ast	*redir_parser(t_token **cur, t_token **errtok)
{
	t_ast	*subtree;
	t_ast	*expr;

	subtree = NULL;
	subtree = _build_rd_subtree(cur, subtree, errtok);
	expr = primary_parser(cur, errtok);
	if (!subtree)
	{
		subtree = expr;
	}
	else 
	{
		if (!insert_expr_leaf(subtree, expr))
			return (ast_free(subtree), NULL);
	}
	return (_build_rd_subtree(cur, subtree, errtok));
}
