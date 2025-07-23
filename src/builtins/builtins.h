/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 05:28:39 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/23 21:30:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdint.h>

# include "ast/ast.h"
# include "env/env.h"

typedef int32_t	(*t_builtin_func)(int32_t ac, char **av, t_sh_ctx *ctx);

bool	get_builtin_func(char *cmd, t_builtin_func *func);
uint8_t	is_builtin(t_sh_ctx *ctx, t_ast *node);

int32_t	main_cd(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_echo(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_env(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_exit(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_export(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_pwd(int32_t ac, char **av, t_sh_ctx *ctx);
int32_t	main_unset(int32_t ac, char **av, t_sh_ctx *ctx);

#endif
