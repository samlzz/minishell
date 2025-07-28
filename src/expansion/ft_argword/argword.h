/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argword.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 17:04:37 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/28 17:56:46 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGWORD_H
# define ARGWORD_H

# include "ft_dyn.h"
# include "lexer/token.h"
# include "env/env.h"

# ifdef MINISHELL_BONUS

typedef struct s_argword
{
	char				*value;
	t_dynint			wild_offsets;
	t_dynint			space_offsets;
	bool				is_expanded;
	struct s_argword	*next;
}	t_argword;

# else

typedef struct s_argword
{
	char				*value;
	t_dynint			space_offsets;
	bool				is_expanded;
	struct s_argword	*next;
}	t_argword;

# endif

t_argword	*argword_new(void);
size_t		argword_size(t_argword *head);
t_argword	*argword_getlast(t_argword *lst);
void		argword_add_back(t_argword **lst, t_argword *new);
void		argword_clear(t_argword *lst);

int16_t		argword_append_value(t_argword *node, const char *cur_arg, \
													t_quote_type cur_quote);

t_argword	*fill_argword(t_sh_ctx *ctx, t_token **cur, bool assign_stop);

# ifdef MINISHELL_BONUS

void		argword_sort(t_argword **head);
t_argword	*argword_detach_next(t_argword *node);
t_argword	**argword_insert(t_argword **cur, t_argword *next, \
															t_argword *node);

# endif

#endif
