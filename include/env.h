/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:32:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/19 17:34:01 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "ft_hmap.h"

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

//? limits
# define ENV_SHLVL_MAX		1000
# define ENV_SHLVL_MIN		0

# define ENV_PRGM_NM		"@ARGV0"
# define ENV_PATH_NM		"@DEFAULT_PATH"
# define ENV_DEFAULT_PATH	"/usr/local/sbin:/usr/local/bin:/usr/bin"

t_hmap	env_init(char **envp, const char *argv0);

#endif
