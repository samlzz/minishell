/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 07:38:14 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/24 03:14:21 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

static inline void	_get_builtin_func(char *cmd, t_builtin_func *func)
{
	const t_builtin_func	funcs[] = {main_cd, main_echo, main_env, main_exit,
		main_export, main_pwd, main_unset};
	const char				*names[] = {"cd", "echo", "env", "exit", "export",
		"pwd", "unset", NULL};
	size_t					i;

	*func = NULL;
	i = -1;
	if (cmd)
		while (names[++i])
			if (!ft_strcmp(names[i], cmd))
				*func = funcs[i];
}

bool	set_builtin_func(t_sh_ctx *ctx, t_ast *node)
{
	t_token	*errtok;

	if (!node)
		return (true);
	else if (node->type == ND_REDIR)
		return (set_builtin_func(ctx, node));
	else if (node->type == ND_PIPE)
		return (set_builtin_func(ctx, node->u_data.op.left)
			&& set_builtin_func(ctx, node->u_data.op.right));
	else if (node->type == ND_CMD)
	{
		errtok = NULL;
		if (!node->u_data.cmd.is_expanded && expand_node(ctx, node, &errtok))
			return (err_print_expand(errtok), context_free(ctx), false);
		_get_builtin_func(node->u_data.cmd.args->expanded,
			&node->u_data.cmd.bi);
	}
	return (true);
}

bool	is_builtin(t_ast *node)
{
	if (!node)
		return (false);
	else if (node->type == ND_REDIR)
		return (is_builtin(node->u_data.rd.child));
	else if (node->type == ND_CMD)
		return (!!node->u_data.cmd.bi);
	return (false);
}
