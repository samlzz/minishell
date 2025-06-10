/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/10 15:06:12 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

//TODO: tmp

void	print_ast_ascii(t_ast *node, bool expanded);
#define EXEC(node, env)	exec_simu(node, env); print_ast_ascii(node, true)

static inline bool	_skipable(const char *input)
{
	size_t	i;

	if (!ft_strncmp(input, "\n", 1))
		return (true);
	if (!ft_strncmp(input, ":", 1))
		return (true);
	if (!ft_strncmp(input, "!", 1))
		return (true);
	i = 0;
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (!input[i]);
}

void	exec_simu(t_ast	*head, t_hmap *env)
{
	if (!head)
		return ;
	t_token	*errtok = NULL;

	if (head->type == ND_AND || head->type == ND_OR || head->type == ND_PIPE)
	{
		exec_simu(head->u_data.op.left, env);
		exec_simu(head->u_data.op.right, env);
	}
	else if (head->type == ND_SUBSHELL)
	{
		exec_simu(head->u_data.subsh.child, env);
	}
	else if (head->type == ND_CMD)
	{
		if (expand_node(head, env, &errtok))
		{
			err_print(PARSE_OK, errtok);
			printf("CMD_ERROR: expand ('%s')\n", head->u_data.cmd.args->expanded);
		}
	}
	else if (head->type == ND_REDIR)
	{
		if (expand_node(head, env, &errtok))
		{
			err_print(PARSE_OK, errtok);
			printf("RD_ERROR: expand ('%s')\n", head->u_data.rd.filename.expanded);
		}
		exec_simu(head->u_data.rd.child, env);
	}
}

void	main_loop(t_hmap *env)
{
	char	*input;
	t_ast	*ast;

	while (1)
	{
		input = readline(CMD_PROMPT);
		if (!input)
		{
			ft_putendl_fd("exit", 2);
			break;
		}
		if (*input)
			add_history(input);
		if (!_skipable(input))
		{
			ast = parse_ast(input);
			if (ast)
			{
				print_ast_ascii(ast, false);
				EXEC(ast, env);
			}
			// TODO: use true when expand are called in exec
			ast_free(ast, true);
		}
		free(input);
	}
}

// TODO: tmp
#include <stdio.h>
void	print_entry(char *key, void *val)
{
	if (*key == '@')
		printf("(INTERNAL):");
	printf("[%s]: %s\n", key, (char *)val);
}

int	main(int argc, char const *argv[], char **envp)
{
	t_hmap	env;

	(void)argc;
	env = env_init(envp, argv[0]);
	if (PRINT_ENV)
		ft_hmap_iter(&env, &print_entry);
	if (!env.__entries)
		return (1);
	main_loop(&env);
	ft_hmap_free(&env, &free);
	return (0);
}
