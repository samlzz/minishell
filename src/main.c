/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/24 09:25:49 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec/exec.h"
#include "minishell.h"
#include "heredoc/here_doc.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

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

#ifdef DEBUG_MODE

static void	_launch_exec(t_hmap *env, const char *input)
{
	t_ast	*ast;

	ast = parse_ast(input);
	if (!ast)
		return ;
	if (PRINT_AST || PRINT_AST_NO_EXPAND)
		print_ast_ascii(ast);
	if (!write_heredocs(ast))
		exec_wrapper(env, ast);
	if (PRINT_AST || PRINT_AST_EXPAND)
		print_ast_ascii(ast);
	ast_free(ast);
}

int	main(int argc, char const *argv[], char **envp)
{
	t_hmap	env;
	char	*input;

	(void)argc;
	env = env_init(envp, argv[0]);
	if (!env.__entries)
		return (1);
	if (PRINT_ENV)
		ft_hmap_iter(&env, &print_entry);
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
			_launch_exec(&env, input);
		free(input);
	}
	ft_hmap_free(&env, &free);
	return (0);
}

#else

static void	_launch_exec(t_hmap *env, const char *input)
{
	t_ast	*ast;

	ast = parse_ast(input);
	if (!ast)
		return ;
	if (!write_heredocs(ast))
		exec_wrapper(env, ast);
	ast_free(ast);
}

int	main(int argc, char const *argv[], char **envp)
{
	t_hmap	env;
	char	*input;

	(void)argc;
	env = env_init(envp, argv[0]);
	if (!env.__entries)
		return (1);
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
			_launch_exec(&env, input);
		free(input);
	}
	ft_hmap_free(&env, &free);
	return (0);
}

#endif