/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:12:41 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/22 21:33:25 by sliziard         ###   ########.fr       */
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
typedef	struct s_dynint		t_dynint;

// parsing
char			*ft_getinput(char const *prompt);

bool			is_redirection(t_tk_type type);
t_redir_type	get_rd_type(t_tk_type tk);

// expand
t_argword		**argword_insert(t_argword **cur, t_argword *next, \
					t_argword *node);
t_argword		*argword_detach_next(t_argword *node);
int16_t			argword_append_value(t_argword *node, const char *cur_arg, \
					t_quote_type cur_quote);

bool			is_wildcard(t_dynint wild_offsets, int32_t i);

// string
bool			ft_isspace(int c);
char			*ft_str3join(const char *s1, const char *s2, const char *s3);
bool			ft_strjreplace(char **target, const char *suff);

#endif
