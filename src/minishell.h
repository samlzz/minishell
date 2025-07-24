/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/24 10:41:00 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# ifdef MINISHELL_BONUS
#  define BONUS_MODE 1
# else
#  define BONUS_MODE 0
# endif

// TODO: tmp (for debug)
# ifdef DEBUG_MODE
#  include "test/test.h"
# endif

# define CMD_PROMPT	"minishell> "

#endif
