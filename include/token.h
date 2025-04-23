/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:37:32 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/23 13:38:53 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

#include "libft.h"

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

t_token		*tokenise(const char *input, int16_t *exit_code);

// ft_token
void		next(t_token **cur);
t_token 	*token_pop(t_token **lst, t_token *to_retrieve);
t_token		*token_dup(t_token *og);
void		token_clear(t_token *lst);
void		token_addback(t_token **lst, t_token *new);

#endif
