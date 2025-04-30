/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:24:55 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 18:35:55 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "ast/ast.h"
# include "heredoc/here_doc.h"
# include "error/error.h"

# define CMD_PROMPT	"minishell> "

t_ast	*parse_line(t_hmap *env, const char *input);

#endif
