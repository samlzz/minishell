/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/20 17:30:13 by sliziard         ###   ########.fr       */
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

static inline void	_print_env(t_env *env)
{
	size_t	i;
	
	i = 0;
	while (i < env->size)
	{
		printf("[%zu]: (%s)\n", i, env->entries[i]);
		i++;
	}
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
	if (PRINT_ENV)
		_print_env(ctx->env);
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
	return (context_free(ctx), ret);
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
