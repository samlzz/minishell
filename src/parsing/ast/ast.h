/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:21:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 17:36:17 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "parser/parser.h"
# include "expansion/expander.h"

//* Types

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

// Data(redir)
typedef struct s_redir
{
	t_redir_type	redir_type;
	char			*filename;
	bool			hd_expand;
	struct s_ast	*child;
}	t_redir;

// Data(binop)
typedef struct s_binop
{
	struct s_ast	*left;
	struct s_ast	*right;
}	t_binop;

typedef struct s_ast
{
	t_node_type	type;
	union
	{
		t_redir	rd;
		t_binop	op;
		struct
		{
			char	**argv;
		}		s_cmd;
		struct
		{
			struct s_ast	*child;
		}		s_subsh;
	}	u_data;
}	t_ast;

//* Functions

t_ast	*new_ast(t_token *tokens, t_token **errtok, int16_t *errcode);
void	ast_free(t_ast	*node);

#endif
