/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:07 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/10 15:27:41 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "ast.h"

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define CMD_PROMPT "minishell> "

t_hmap	env_init(char **envp);

#endif
