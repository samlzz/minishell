/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:04:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 10:23:22 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOC_H
# define HERE_DOC_H

# include "ast/ast.h"

# define HD_PROMPT			"> "
# define HD_CHARSET			"abcdefghijklmnopqrstuvwxyz\
ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
# define HD_CHARSET_LEN		62
# define HD_FNAME_BASE		"/tmp/.minishell_heredoc_"
# define HD_FN_LEN			24

// *Functions

uint8_t	hd_init(t_ast *node);

// utils

uint8_t	hd_rec_init(t_ast *node);
uint8_t	hd_process(t_redir *hd);

t_token	*hd_tokenise(const char *input);

int16_t	hd_quotes_removing(t_redir *node);

// ? For expand (at exec)
void	hd_expand_line(t_sh_ctx *ctx, char **line);
int32_t	hd_gen_filename(char *dest);
int32_t	ft_pipe(int fds[2]);

#endif
