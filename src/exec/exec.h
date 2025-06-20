/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 18:19:12 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/20 12:07:05 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "ast/ast.h"

uint8_t	exec_wrapper(t_hmap *env, t_ast *node);

uint8_t	exec_flow_exec(t_hmap *env, t_ast *root, t_ast *node, int32_t fds[2]);
void	exec_flow_pipe(t_hmap *env, t_ast *root, t_ast *node, int32_t fds[3]);
void	exec_flow_cmd(t_hmap *env, t_ast *root, t_ast *node, int32_t fds[2]);

char	*exec_get_cmd_path(char **av, t_hmap *envp);

#endif
