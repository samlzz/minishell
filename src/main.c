/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 16:56:15 by sliziard          #+#    #+#             */
/*   Updated: 2025/08/01 11:35:09 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/history.h>
#include <unistd.h>

#include "libft.h"
#include "minishell.h"
#include "env/env.h"
#include "handler/handler.h"
#include "utils.h"
#include "heredoc/here_doc.h"
#include "exec/exec.h"

static inline bool	_skipable(const char *input)
{
	size_t	i;

	if (!*input || !ft_strcmp(input, "\n"))
		return (true);
	add_history(input);
	i = 0;
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (input[i] == '\0');
}

static uint8_t	_launch_exec(t_sh_ctx *ctx, const char *input)
{
	uint8_t	ret;
	t_ast	*ast;

	ast = parse_ast(input);
	if (!ast)
		return (ctx->lst_exit = 2);
	g_sig = 0;
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
	ctx = context_init(envp, argv[0]);
	if (!ctx)
		return (ret = 1, 1);
	sig_init(SIGH_MAIN);
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
		return (_launch_exec(ctx, argv[2]));
	while (1)
	{
		input = ft_getinput(CMD_PROMPT, true);
		if (!input)
			break ;
		if (!_skipable(input))
			ret = _launch_exec(ctx, input);
		free(input);
		if (ctx->exit)
			break ;
	}
	ft_close_all(NULL);
	return (context_free(ctx), ret);
}
