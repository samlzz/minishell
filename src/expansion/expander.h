/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:18:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 10:18:44 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "ft_dyn.h"
# include "env/env.h"
# include "ast/ast.h"
# include "lexer/token.h"

// * Argwords

typedef struct s_argword
{
	char				*value;
	t_dynint			wild_offsets;
	t_dynint			space_offsets;
	bool				is_expanded;
	struct s_argword	*next;
}	t_argword;

// Ft_argword
t_argword	*argword_new(void);
size_t		argword_size(t_argword *head);
t_argword	*argword_getlast(t_argword *lst);
void		argword_add_back(t_argword **lst, t_argword *new);
void		argword_clear(t_argword *lst);

void		argword_sort(t_argword **head);

// utils
t_argword	**argword_insert(t_argword **cur, t_argword *next, \
													t_argword *node);
t_argword	*argword_detach_next(t_argword *node);
int16_t		argword_append_value(t_argword *node, const char *cur_arg, \
													t_quote_type cur_quote);

bool		is_wildcard(t_dynint wild_offsets, int32_t i);

// * Expand

// ? tild expand
char		*ft_getuser(void);
char		*ft_gethome(const char *username);
void		expand_tild(t_token *cur, t_env *env);
void		expand_tild_export(t_token *argv, t_env *env);

t_argword	*fill_argword(t_sh_ctx *ctx, t_token **cur, bool assign_stop);
t_argword	*expand_word(t_sh_ctx *ctx, t_token **cur, bool split, \
														bool assign_stop);
bool		is_export_cmd(t_token *argv);
t_argword	*expand_export_cmd(t_token *cur, t_sh_ctx *ctx);

int16_t		expand_node(t_sh_ctx *ctx, t_ast *node, t_token **errtok);

t_argword	*split_withespaces(t_argword *field);
t_argword	*replace_wildcards(t_argword *head);

#endif
