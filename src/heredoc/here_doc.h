/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:04:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/21 10:29:39 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HERE_DOC_H
# define HERE_DOC_H

# include "ast/ast.h"

# define HD_PROMPT			"> "
# define HD_CHARSET			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
# define HD_CHARSET_LEN		62
# define HD_FNAME_BASE		"/tmp/.minishell_heredoc_"
# define HD_FN_LEN			24

//* Functions

int16_t	write_heredocs(t_ast *node);
void	expand_line(t_hmap *env, char **line);
int32_t	gen_heredoc_filename(char *dest);

t_token	*hd_tokenise(const char *input);

#endif
