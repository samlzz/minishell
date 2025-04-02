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
# include <stdint.h>

typedef enum e_tk_type
{
	TK_WORD,
	TK_PIPE,
	TK_AND,
	TK_OR,
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
	struct s_token	*next;
}	t_token;

typedef struct s_ast	t_ast;

t_ast	*binop_parser(t_token **cur);
t_ast	*cmd_parser(t_token **cur);

t_token	*tokenise(char *input);

//tokens
void	token_clear(t_token *lst);
t_token	*token_last(t_token *lst);
void	token_addfront(t_token **lst, t_token *new);
void	token_addback(t_token **lst, t_token *new);

#endif
