/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/21 12:18:57 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

//TODO: tmp
void	print_ast_ascii(t_ast *node);
#define EXEC(node)	print_ast_ascii(node)

void	get_input(t_hmap *env)
{
	char	*input;
	int16_t	err;
	t_token	*errtok;
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
		ast = new_ast(env, input, &err, &errtok);
		if (!ast)
			print_err(err, errtok);
		else
			EXEC(ast);
		free(input);
		ast_free(ast);
	}
}

// TODO: tmp
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
	ft_hmap_iter(&env, &print_entry);
	if (!env.__entries)
		return (1);
	get_input(&env);
	ft_hmap_free(&env, &free);
	return (0);
}
