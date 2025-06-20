/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:12:41 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/10 12:07:36 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdbool.h>
# include <stdint.h>

//? Forward declaration
typedef enum e_tk_type		t_tk_type;
typedef enum e_quote_type	t_quote_type;
typedef enum e_redir_type	t_redir_type;

typedef struct s_argword	t_argword;

// parsing
bool			is_redirection(t_tk_type type);
t_redir_type	get_rd_type(t_tk_type tk);

// argword
t_argword		**argword_insert(t_argword **cur, t_argword \
				*next, t_argword *node);
t_argword		*argword_detach_next(t_argword *node);

int16_t			argword_append_value(t_argword *node, const char *cur_arg, \
				t_quote_type cur_quote);

// string
bool			ft_isspace(int c);
char			*ft_str3join(const char *s1, const char *s2, const char *s3);

#endif
