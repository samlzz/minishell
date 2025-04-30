/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 18:38:01 by sliziard         ###   ########.fr       */
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
			ast = parse_line(env, input);
			if (ast)
				EXEC(ast);
			ast_free(ast);
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
