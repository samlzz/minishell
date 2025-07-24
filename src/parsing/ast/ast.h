/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:21:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/24 11:07:35 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include <sys/types.h>

# include "parser/parser.h"

typedef struct s_sh_ctx	t_sh_ctx;
typedef int32_t	(*t_builtin_func)(int32_t ac, char **av, t_sh_ctx *ctx);

//* Types

typedef enum e_redir_type
{
	RD_IN,
	RD_HEREDOC,
	RD_OUT,
	RD_APPEND
}	t_redir_type;

typedef union u_words
{
	t_token	*tk;
	char	*expanded;
}	t_words;

typedef struct s_expr
{
	uint8_t	ret;
	pid_t	pid;
}	t_expr;

// Data(redir)
typedef struct s_redir
{
	t_redir_type	redir_type;
	t_words			filename;
	int32_t			fd;
	bool			hd_expand;
	bool			is_expanded;
	struct s_ast	*child;
	t_expr			exec_infos;
}	t_redir;

// Data(binop)
typedef struct s_binop
{
	struct s_ast	*left;
	struct s_ast	*right;
}	t_binop;

// Data (cmd)
typedef struct s_cmd
{
	t_words			*args;
	bool			is_expanded;
	t_expr			exec_infos;
	t_builtin_func	bi;
}	t_cmd;

# ifdef MINISHELL_BONUS

typedef enum e_node_type
{
	ND_CMD,
	ND_PIPE,
	ND_AND,
	ND_OR,
	ND_REDIR,
	ND_SUBSHELL
}	t_node_type;

// Data (subshell)
typedef struct s_subshell
{
	struct s_ast	*child;
	t_expr			exec_infos;
}	t_subshell;

typedef struct s_ast
{
	t_node_type	type;
	union
	{
		t_redir		rd;
		t_binop		op;
		t_cmd		cmd;
		t_subshell	subsh;
	}	u_data;
}	t_ast;

# else

typedef enum e_node_type
{
	ND_CMD,
	ND_PIPE,
	ND_REDIR,
}	t_node_type;

typedef struct s_ast
{
	t_node_type	type;
	union
	{
		t_redir		rd;
		t_binop		op;
		t_cmd		cmd;
	}	u_data;
}	t_ast;

# endif

//* Functions

t_ast	*parse_ast(const char *input);

t_ast	*new_ast(t_token *tokens, t_token **errtok, int16_t *errcode);
void	ast_free(t_ast *node);

#endif
