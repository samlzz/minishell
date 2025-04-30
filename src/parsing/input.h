/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:24:55 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 17:59:38 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "ast/ast.h"
# include "heredoc/here_doc.h"
# include "error/error.h"

# define CMD_PROMPT	"minishell> "

void	get_input(t_hmap *env);
t_ast	*parse_input(t_hmap *env, const char *input, int16_t *err_code, \
	t_token **errtok);

#endif
