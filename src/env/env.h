/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:32:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/25 22:07:45 by mle-flem         ###   ########.fr       */
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

# define ENV_DEFAULT_PATH	"/usr/local/sbin:/usr/local/bin:/usr/bin"

typedef struct s_sh_ctx
{
	t_hmap		env;
	uint8_t		lst_exit;
	const char	*p_name;
	char		*p_id;
	bool		use_fallback_path;
}	t_sh_ctx;

//* Functions

t_sh_ctx	*context_init(char **envp, const char *argv0);
t_hmap		env_init(char **envp);

// Wrappers
void	context_free(t_sh_ctx *ctx);

char	**get_envp(t_hmap *env);
int16_t	env_set(t_hmap *env, const char *key, char *value);
int16_t	env_literal_set(t_hmap *env, const char *key, const char *val);

#endif
