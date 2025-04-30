/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:23:53 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 16:55:40 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

#include "ft_dyn.h"
#include "ft_hmap.h"
#include "lexer/token.h"
#include "utils/utils.h"

typedef struct s_argword
{
	char				*value;
	t_dynint			wild_offsets;
	t_dynint			space_offsets;
	struct s_argword	*next;
}	t_argword;

//* Functions

// Ft_argword
t_argword	*argword_new(void);
int16_t		argword_append_value(t_argword *node, const char *cur_arg, \
		t_quote_type cur_quote);
void		argword_add_back(t_argword **lst, t_argword *new);
void		argword_clear(t_argword *lst);

//* Expansion

t_token		*expand_token_list(t_hmap *env, t_token *lst);

t_argword	*build_argword(t_hmap *env, t_token **cur, int16_t lst_exit);
t_argword	*split_withespace(t_argword *arg);
// wildcard
t_argword	*replace_by_wild_expanded(t_argword *head);

#endif
