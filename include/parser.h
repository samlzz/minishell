/* ************************************************************************** */
/*	                                                                        */
/*	                                                    :::      ::::::::   */
/*   parser.h	                                       :+:      :+:    :+:   */
/*	                                                +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>	      +#+  +:+       +#+        */
/*	                                            +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 19:56:14 by sliziard	      #+#    #+#             */
/*   Updated: 2025/03/31 19:56:15 by sliziard	     ###   ########.fr       */
/*	                                                                        */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "ft_hmap.h"
# include "ft_dyn.h"
# include <stdint.h>

# define HANDLED_CHAR	"()<>|&'\""

//? Error codes
# define PARSE_OK			0
# define PARSE_ERR			-1
# define PARSE_ERR_SQUOTE	-2
# define PARSE_ERR_DQUOTE	-3
# define PARSE_ERR_EOF		-4

//* Tokens types

typedef enum e_tk_type
{
	TK_WORD,
	TK_PIPE,
	TK_AND,
	TK_OR,
	TK_LPAREN,
	TK_RPAREN,
	TK_REDIR_IN,
	TK_REDIR_OUT,
	TK_REDIR_APPEND,
	TK_HEREDOC,
	TK_EOF
}	t_tk_type;

typedef	enum e_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef struct s_token
{
	t_tk_type		type;
	t_quote_type	quote;
	char			*value;
	bool			glued;
	struct s_token	*next;
}	t_token;

typedef struct s_argword
{
	char				*value;
	t_dynint			wild_offsets;
	t_dynint			space_offsets;
	bool				no_quote;
	struct s_argword	*next;
}	t_argword;

//? Forward declaration
typedef struct s_ast	t_ast;
typedef enum e_node_type	t_node_type;

//* Main Functions

t_token		*tokenise(const char *input, int16_t *exit_code);

// expander
t_argword	*build_argword(t_hmap *env, t_token **cur, int16_t lst_exit);
t_argword	*expand_wildcards(t_argword *arg);
t_token		*expand_token_list(t_hmap *env, t_token *lst, t_token **errtok);

// parser
t_ast		*binop_parser(t_token **cur, t_node_type bin_op, \
		t_token **errtok);
t_ast		*redir_parser(t_token **cur, t_token **errtok);
t_ast		*primary_parser(t_token **cur, t_token **errtok);

//* Helper functions
//? Linked list

// argword (for expand)
t_argword	*argword_new(void);
int16_t		argword_append_value(t_argword *node, const char *cur_arg, \
		t_quote_type cur_quote);
void		argword_add_back(t_argword **lst, t_argword *new);
void		argword_clear(t_argword *lst);

void	argword_sort_alpha(t_argword **head);

// tokens
void		next(t_token **cur);
t_token 	*token_pop(t_token **lst, t_token *to_retrieve);
t_token		*token_dup(t_token *og);
void		token_clear(t_token *lst);
void		token_addback(t_token **lst, t_token *new);

#endif
