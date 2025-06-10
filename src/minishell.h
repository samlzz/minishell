/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/24 09:24:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "env/env.h"
# include "ast/ast.h"

// TODO: tmp (for debug)
# ifdef DEBUG_MODE
#  include "test/test.h"
# endif

# define CMD_PROMPT	"minishell> "

#endif
