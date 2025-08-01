/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:39 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/31 11:22:45 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdint.h>

# include "ast/ast.h"
# include "env/env.h"

typedef int32_t	(*t_builtin_func)(int32_t ac, char **av, t_sh_ctx *ctx);

bool	set_builtin_func(t_sh_ctx *ctx, t_ast *node);
bool	is_builtin(t_ast *node);

bool	bi_putstr(char *cmd, char *str);
bool	bi_putendl(char *cmd, char *str);

int32_t	main_cd(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_echo(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_env(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_exit(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_export(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_pwd(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_unset(int32_t ac, char **av, t_sh_ctx *ctx);

// export helpers
bool	check_invalid_opt(int32_t *ac, char ***av);
bool	is_valid_env_name(char *name, bool print_err);
void	export_sort_envp(char **envp);

// cd helpers
char	*cd_absolute(t_sh_ctx *ctx, char *path);
char	*cd_canonicalize(char *path);

#endif
