/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_writter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:08:12 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/02 16:53:40 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include "env/env.h"
#include "exec/exec.h"
#include <stdio.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>

static inline void	_hd_read(int write_fd, char const *delim)
{
	char	*line;

	while (1)
	{
		line = readline(HD_PROMPT);
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break;
		}
		ft_putendl_fd(line, write_fd);
		free(line);
	}
}

static int16_t	_hd_process(t_redir *hd)
{
	int	fds[2];

	if (ft_pipe(fds) == -1)
		return (perror("minishell: hd_process: pipe"), 1);
	if (hd_quotes_removing(hd))
	{
		close(fds[0]);
		close(fds[1]);
		return (1);
	}
	hd->fd = fds[0];
	_hd_read(fds[1], hd->filename.expanded);
	close(fds[1]);
	return (0);
}

static int16_t	_hd_rec_init(t_ast *node, t_sh_ctx *ctx)
{
	if (!node)
		return (0);
	else if (node->type == ND_REDIR && node->u_data.rd.redir_type == RD_HEREDOC)
	{
		if (_hd_process(&node->u_data.rd))
			return (1);
		return (_hd_rec_init(node->u_data.rd.child, ctx));
	}
	else if (node->type == ND_REDIR)
		return (_hd_rec_init(node->u_data.rd.child, ctx));
	else if (node->type == ND_PIPE || node->type == ND_AND || node->type == ND_OR)
	{
		if (_hd_rec_init(node->u_data.op.left, ctx))
			return (1);
		return (_hd_rec_init(node->u_data.op.right, ctx));
	}
	else if (node->type == ND_SUBSHELL)
		return (_hd_rec_init(node->u_data.subsh.child, ctx));
	return (0);
}

int16_t	hd_init(t_ast *head, t_sh_ctx *ctx)
{
	return (_hd_rec_init(head, ctx));
}