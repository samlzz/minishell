/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:19:12 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/27 08:23:23 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "ast/ast.h"

# ifndef MINISHELL_BONUS
#  define ND_SUBSHELL 42
#  define ND_AND 42
#  define ND_OR 42
# endif

uint8_t	exec_wrapper(t_sh_ctx *ctx, t_ast *node);

uint8_t	exec_flow_exec(t_sh_ctx *ctx, t_ast *node, int32_t fds[2]);
void	exec_flow_pipe(t_sh_ctx *ctx, t_ast *node, int32_t fds[3]);
void	exec_flow_cmd(t_sh_ctx *ctx, t_ast *node, int32_t fds[2]);
void	exec_flow_redir(t_sh_ctx *ctx, t_ast *node, int32_t fds[2]);

char	*exec_get_cmd_path(char **av, t_sh_ctx *ctx);
size_t	exec_wait_get_count(t_ast *node, bool is_hd);

void	ft_close_pipe(int32_t *fds);
void	ft_close_all(int32_t *fds);
void	ft_dup_fds(int32_t fds[2]);

void	rd_exit_clean(uint8_t code, t_sh_ctx *ctx, t_ast *node, int32_t fds[2]);

void	exec_wait_set_ret(t_ast *node, pid_t pid, uint8_t ret, int32_t status);
void	print_sig_message(uint8_t sig, bool coredump);

#endif
