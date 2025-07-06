/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 07:38:14 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/06 09:38:25 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

bool	get_builtin_func(char *cmd, t_builtin_func *func)
{
	const t_builtin_func	funcs[] = {main_cd, main_echo, main_env, main_exit,
		main_export, main_pwd, main_unset};
	const char				*names[] = {"cd", "echo", "env", "exit", "export",
		"pwd", "unset", NULL};
	size_t					i;

	*func = NULL;
	i = -1;
	while (names[++i])
		if (!ft_strcmp(names[i], cmd))
			*func = funcs[i];
	return (!!*func);
}

uint8_t	is_builtin(t_sh_ctx *ctx, t_ast *root, t_ast *node)
{
	t_token			*errtok;
	t_builtin_func	func;

	if (!node)
		return (0);
	if (node->type == ND_REDIR)
		return (is_builtin(ctx, root, node->u_data.rd.child));
	else if (node->type == ND_CMD)
	{
		errtok = NULL;
		if (expand_node(ctx, node, &errtok))
			return (err_print_expand(errtok), context_free(ctx), ast_free(root),
				2);
		else if (get_builtin_func(node->u_data.cmd.args->expanded, &func))
			return (1);
	}
	return (0);
}
