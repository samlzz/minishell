/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:59:06 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 18:00:19 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include "lexer/token.h"

//? Error codes
# define PARSE_OK				0
# define PARSE_ERR				-1
# define PARSE_ERR_SQUOTE		-2
# define PARSE_ERR_DQUOTE		-3
# define PARSE_ERR_MALFORMED	-4
# define PARSE_ERR_SOLO_AND		-6
# define PARSE_ERR_EOF			-5

void	print_err(int16_t errcode, t_token *errtok);

#endif
