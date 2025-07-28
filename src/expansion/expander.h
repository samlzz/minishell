/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:18:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/28 17:56:56 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "ft_argword/argword.h"
# include "ast/ast.h"

int16_t		expand_node(t_sh_ctx *ctx, t_ast *node, t_token **errtok);

t_argword	*expand_word(t_sh_ctx *ctx, t_token **cur, bool split, \
														bool is_export);
t_argword	*expand_export_cmd(t_token *cur, t_sh_ctx *ctx);

// *Tild expand
char		*ft_getuser(void);
char		*ft_gethome(const char *username);
void		expand_tild(t_token *cur, t_env *env);
void		expand_tild_export(t_token *argv, t_env *env);

// *Field splitting
t_argword	*field_splitting(t_argword *field);

# ifdef MINISHELL_BONUS

// *Wildcards
t_argword	*replace_wildcards(t_argword *head);
t_argword	*export_wildcard_handler(t_argword *args, t_argword *entry, \
																t_token *cur);

// *Utils
bool		is_wildcard(t_dynint wild_offsets, int32_t i);
# endif

bool		is_export_cmd(t_token *argv);

#endif
