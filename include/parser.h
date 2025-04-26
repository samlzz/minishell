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

# include "ft_hmap.h"
# include "ft_dyn.h"
# include "token.h"

# define HANDLED_CHAR	"()<>|&'\""

//? Error codes
# define PARSE_OK				0
# define PARSE_ERR				-1
# define PARSE_ERR_SQUOTE		-2
# define PARSE_ERR_DQUOTE		-3
# define PARSE_ERR_MALFORMED	-4
# define PARSE_ERR_SOLO_AND		-6
# define PARSE_ERR_EOF			-5

//* Tokens types

typedef struct s_argword
{
	char				*value;
	t_dynint			wild_offsets;
	t_dynint			space_offsets;
	struct s_argword	*next;
}	t_argword;

//? Forward declaration
typedef struct s_ast	t_ast;
typedef enum e_node_type	t_node_type;

//* Main Functions

// expander
t_token		*expand_token_list(t_hmap *env, t_token *lst, t_token **errtok);

t_argword	*split_withespace(t_argword *arg);
t_argword	*replace_by_wild_expanded(t_argword *head);
t_argword	*expand_wildcards(t_argword *arg);

// parser
t_ast		*binop_parser(t_token **cur, t_node_type bin_op, \
		t_token **errtok);
t_ast		*redir_parser(t_token **cur, t_token **errtok);
t_ast		*primary_parser(t_token **cur, t_token **errtok);
char		**join_argv(char **og, char **new);
char		**collect_argv(t_token **cur, t_token **errtok);

//* Helper functions

// argword (for expand)
t_argword	*build_argword(t_hmap *env, t_token **cur, int16_t lst_exit);

t_argword	*argword_new(void);
int16_t		argword_append_value(t_argword *node, const char *cur_arg, \
		t_quote_type cur_quote);
void		argword_add_back(t_argword **lst, t_argword *new);
void		argword_clear(t_argword *lst);

void		argword_sort_alpha(t_argword **head);

//utils
bool		ft_isspace(int c);
char		*ft_str3join(const char *s1, const char *s2, const char *s3);
bool		is_redirection(t_tk_type type);

#endif
