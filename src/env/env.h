/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:32:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/25 10:14:20 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

//? limits
# define ENV_SHLVL_MAX		1000
# define ENV_SHLVL_MIN		0

# define ENV_MINIMAL_SIZE	4

# define ENV_DEFAULT_PATH	"/usr/local/sbin:/usr/local/bin:/usr/bin"

typedef struct s_ast	t_ast;

typedef struct s_env
{
	char	**entries;
	size_t	size;
	size_t	cap;
}	t_env;

typedef struct s_sh_ctx
{
	t_env		*env;
	uint8_t		lst_exit;
	const char	*p_name;
	char		*p_id;
	t_ast		*head;
	bool		use_fallback_path;
	bool		exit;
}	t_sh_ctx;

// * Ft Context

t_sh_ctx	*context_init(char **envp, const char *argv0);
void		context_free(t_sh_ctx *ctx);

// * Ft Env

t_env		*env_init(char *const envp[]);
char		*env_get(t_env *env, const char *key);
char		**env_get_envp(t_env *env, char *cmd, bool is_export);

int16_t		env_set(t_env *env, char *entry);
int16_t		env_literal_set(t_env *env, const char *key, const char *val);
void		env_rm(t_env *env, const char *key);

#endif
