/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:59:06 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/26 12:12:29 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "lexer/token.h"
# include <errno.h>

//? Error codes
# define PARSE_OK				0
# define PARSE_ERR				-1
# define PARSE_ERR_SQUOTE		-2
# define PARSE_ERR_DQUOTE		-3
# define PARSE_ERR_MALFORMED	-4
# define PARSE_ERR_SOLO_AND		-6
# define PARSE_ERR_EOF			-5

void	err_print(int16_t errcode, t_token *errtok, bool is_tokenize);
void	err_print_expand(t_token *errtok);

#endif
