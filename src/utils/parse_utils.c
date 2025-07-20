/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:08:06 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/20 17:43:36 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "ast/ast.h"
#include "lexer/token.h"
#include "ft_gnl.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

bool	is_redirection(t_tk_type type)
{
	return (type == TK_REDIR_IN || type == TK_REDIR_OUT
		|| type == TK_REDIR_APPEND || type == TK_HEREDOC);
}

t_redir_type	get_rd_type(t_tk_type tk)
{
	if (tk == TK_REDIR_IN)
		return (RD_IN);
	if (tk == TK_REDIR_OUT)
		return (RD_OUT);
	if (tk == TK_REDIR_APPEND)
		return (RD_APPEND);
	if (tk == TK_HEREDOC)
		return (RD_HEREDOC);
	return (-1);
}

char	*ft_getinput(char const *prompt)
{
	size_t	len;
	char	*input;

	if (isatty(STDIN_FILENO))
	{
		rl_outstream = stderr;
		input = readline(prompt);
		if (!input)
			return (ft_putendl_fd("exit", 2), NULL);
		if (*input)
			add_history(input);
	}
	else
	{
		ft_getline(&input, STDIN_FILENO);
		if (!input)
			return (NULL);
		len = ft_strlen(input) - 1;
		if (input[len] == '\n')
			input[len] = '\0';
	}
	return (input);
}
