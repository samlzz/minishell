/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:21:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/07 14:03:01 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "parser.h"

typedef enum e_node_type
{
	ND_CMD,
	ND_PIPE,
	ND_AND,
	ND_OR,
	ND_REDIR,
	ND_SUBSHELL
}	t_node_type;

typedef enum e_redir_type
{
	RD_IN,
	RD_HEREDOC,
	RD_OUT,
	RD_APPEND
}	t_redir_type;

typedef struct s_ast
{
	t_node_type	type;
	union
	{
		struct
		{
			char	**argv;
		} s_cmd;
		struct
		{
			t_redir_type	redir_type;
			char			*filename;
			struct s_ast	*child;
		} s_redir;
		struct
		{
			struct s_ast	*left;
			struct s_ast	*right;
		} s_binop;
		struct
		{
			struct s_ast	*child;
		} s_subsh;
	}	u_data;
}	t_ast;

void	ast_free(t_ast	*node);
t_ast	*new_ast(const char *input, int16_t *errcode, t_token **errtok);

#endif
