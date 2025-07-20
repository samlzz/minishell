/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:19:12 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/20 20:23:58 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "ast/ast.h"
# include "builtins/builtins.h"

uint8_t	exec_wrapper(t_sh_ctx *ctx, t_ast *node);

uint8_t	exec_flow_exec(t_sh_ctx *ctx, t_ast *root, t_ast *node, int32_t fds[2]);
void	exec_flow_pipe(t_sh_ctx *ctx, t_ast *root, t_ast *node, int32_t fds[3]);
void	exec_flow_cmd(t_sh_ctx *ctx, t_ast *root, t_ast *node, int32_t fds[2]);
void	exec_flow_redir(t_sh_ctx *ctx, t_ast *root, t_ast *node, \
			int32_t fds[2]);
void	exec_flow_builtin(t_sh_ctx *ctx, t_ast *root, t_ast *node, \
			int32_t fds[2]);

char	*exec_get_cmd_path(char **av, t_sh_ctx *ctx);
size_t	exec_wait_get_count(t_ast *node, bool is_hd);

#endif
