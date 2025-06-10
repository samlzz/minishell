/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:18:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/10 11:07:17 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "ft_dyn.h"
# include "env/env.h"
# include "ast/ast.h"
# include "lexer/token.h"
# include "utils/utils.h"

// * Argwords

typedef struct s_argword
{
	char				*value;
	t_dynint			wild_offsets;
	t_dynint			space_offsets;
	struct s_argword	*next;
}	t_argword;

// Ft_argword
t_argword	*argword_new(void);
size_t		argword_size(t_argword *head);
void		argword_add_back(t_argword **lst, t_argword *new);
void		argword_clear(t_argword *lst);

void		argword_sort(t_argword **head);

// * Expand

t_argword	*fill_argword(t_hmap *env, t_token **cur, int16_t lst_exit);
t_argword	*expand_word(t_token **cur, bool split, t_hmap *env);

int16_t		expand_node(t_ast *node, t_hmap * env, t_token **errtok);

t_argword	*split_withespaces(t_argword *field);
t_argword	*replace_wildcards(t_argword *head);

#endif
