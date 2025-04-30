/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:32:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 17:12:05 by sliziard         ###   ########.fr       */
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

//* Functions

t_hmap	env_init(char **envp, const char *argv0);

// Wrappers
char	**get_envp(t_hmap *env);
int16_t	env_set(t_hmap *env, const char *key, char *value);
int16_t	env_literal_set(t_hmap *env, const char *key, const char *val);

#endif
