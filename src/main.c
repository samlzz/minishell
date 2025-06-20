/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/24 09:25:26 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec/exec.h"
#include "minishell.h"
#include "heredoc/here_doc.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void	print_ast_ascii(t_ast *node, bool expanded);

// TODO: tmp
#define EXEC(node) print_ast_ascii(node, true)

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

int16_t	expander_simu(t_ast	*head, t_hmap *env)
{
	t_token	*errtok = NULL;

	if (!head)
		return (0);
	if (head->type == ND_AND || head->type == ND_OR || head->type == ND_PIPE)
		return (
			expander_simu(head->u_data.op.left, env), 
			expander_simu(head->u_data.op.right, env)
		);
	else if (head->type == ND_SUBSHELL)
		return (expander_simu(head->u_data.subsh.child, env));
	else if (head->type == ND_CMD && expand_node(head, env, &errtok))
	{
		err_print_expand(errtok);
		printf("CMD_ERROR: expand ('%s')\n", head->u_data.cmd.args->expanded);
		return (1);
	}
	else if (head->type == ND_REDIR)
	{
		if (expand_node(head, env, &errtok))
		{
			err_print_expand(errtok);
			printf("RD_ERROR: expand ('%s')\n", head->u_data.rd.filename.expanded);
			return (1);
		}
		return (expander_simu(head->u_data.rd.child, env));
	}
	return (0);
}

static void	_launch_exec(t_hmap *env, const char *input)
{
	t_ast	*ast;

	ast = parse_ast(input);
	if (!ast)
		return ;
	expander_simu(ast, env);
	if (!write_heredocs(ast))
		EXEC(ast);
	ast_free(ast, true);
}

static void	_launch_exec(t_hmap *env, const char *input)
{
	t_ast	*ast;

	ast = parse_ast(input);
	if (!ast)
		return ;
	# ifdef DEBUG_MODE
	if (PRINT_AST || PRINT_AST_NO_EXPAND)
		print_ast_ascii(ast, false);
	# endif
	// expander_simu(ast, env);
	# ifdef DEBUG_MODE
	if (PRINT_AST || PRINT_AST_EXPAND)
		print_ast_ascii(ast, true);
	# endif
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
	# ifdef DEBUG_MODE
	if (PRINT_ENV)
		ft_hmap_iter(&env, &print_entry);
	# endif
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
