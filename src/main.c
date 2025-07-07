/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/18 15:52:05 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec/exec.h"
#include "ft_gnl.h"
#include "minishell.h"
#include "heredoc/here_doc.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

static inline bool	_skipable(const char *input, t_sh_ctx *ctx)
{
	size_t	i;

	if (!*input || !ft_strcmp(input, "\n"))
		return (true);
	if (!ft_strcmp(input, ":"))
		return (ctx->lst_exit = 0, true);
	if (!ft_strcmp(input, "!"))
		return (ctx->lst_exit = 1, true);
	i = 0;
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	if (!input[i])
		return (ctx->lst_exit = 0, true);
	return (false);
}

#ifdef DEBUG_MODE

static uint8_t	_launch_exec(t_sh_ctx *ctx, const char *input)
{
	uint8_t	ret;
	t_ast	*ast;

	ast = parse_ast(input);
	if (!ast)
		return (ctx->lst_exit = 2);
	if (PRINT_AST || PRINT_AST_NO_EXPAND)
		print_ast(ast);
	ret = hd_init(ast);
	if (!ret)
		ret = exec_wrapper(ctx, ast);
	else
		ctx->lst_exit = ret;
	if (PRINT_AST || PRINT_AST_EXPAND)
		print_expanded_ast(ast, ctx);
	ast_free(ast);
	return (ret);
}

int	main(int argc, char const *argv[], char **envp)
{
	t_sh_ctx	*ctx;
	char		*input;
	uint8_t		ret;

	ret = 0;
	ctx	= context_init(envp, argv[0]);
	if (!ctx)
		return (ret = 1, 1);
	sig_init(SIGH_MAIN);
	if (PRINT_ENV)
		ft_hmap_iter(&ctx->env, &print_entry);
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
		return (_launch_exec(ctx, argv[2]));
	while (1)
	{
		if (isatty(STDIN_FILENO))
		{
			rl_outstream = stderr;
			input = readline(CMD_PROMPT);
			if (!input)
			{
				ft_putendl_fd("exit", 2);
				break;
			}
			if (*input)
				add_history(input);
		}
		else
		{
			ft_getline(&input, STDIN_FILENO);
			if (!input)
				break;
			if (input[ft_strlen(input)] == '\n')
				input[ft_strlen(input)] = 0;
		}
		if (!_skipable(input, ctx))
			ret = _launch_exec(ctx, input);
		free(input);
		input = ft_itoa((int32_t)ret);
		if (input)
			env_set(&ctx->env, "?", input);
	}
	context_free(ctx);
	return (ret);
}

#else

static uint8_t	_launch_exec(t_sh_ctx *ctx, const char *input)
{
	uint8_t	ret;
	t_ast	*ast;

	ast = parse_ast(input);
	if (!ast)
		return (ctx->lst_exit = 2);
	ret = hd_init(ast);
	if (!ret)
		ret = exec_wrapper(ctx, ast);
	else
		ctx->lst_exit = ret;
	ast_free(ast);
	return (ret);
}

int	main(int argc, char const *argv[], char **envp)
{
	t_sh_ctx	*ctx;
	char		*input;
	uint8_t		ret;

	ret = 0;
	ctx	= context_init(envp, argv[0]);
	if (!ctx)
		return (ret = 1, 1);
	sig_init(SIGH_MAIN);
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
		return (_launch_exec(ctx, argv[2]));
	while (1)
	{
		input = ft_getinput(CMD_PROMPT);
		if (!input)
			break ;
		if (!_skipable(input, ctx))
			ret = _launch_exec(ctx, input);
		free(input);
		if (ctx->exit)
			break ;
	}
	context_free(ctx);
	return (ret);
}

#endif
