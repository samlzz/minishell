/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 17:31:56 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "env/env.h"
#include "input.h"

//TODO: tmp (for debug)
# ifdef DEBUG_MODE
#  define PRINT_ENV		1
#  define KEEP_HD_FILES	1
# else
#  define PRINT_ENV		0
#  define KEEP_HD_FILES	0
# endif


#endif
