/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_writter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:08:12 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/30 09:13:17 by sliziard         ###   ########.fr       */
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

static int16_t	_hd_process(t_redir *hd, t_sh_ctx *ctx, t_ast *head)
{
	int	fds[2];

	if (pipe(fds) == -1)
		return (perror("minishell: hd_process: pipe"), 1);
	hd->fd = fds[0];
	hd->exec_infos.pid = fork();
	if (hd->exec_infos.pid == -1)
		return (perror("minishell: hd_process: fork"),
		close(fds[0]), close(fds[1]), 1);
	if (!hd->exec_infos.pid)
	{
		close(fds[0]);
		hd->fd = -1;
		if (hd_quotes_removing(hd))
			(close(fds[1]), exit(1));
		_hd_read(fds[1], hd->filename.expanded);
		close(fds[1]);
		ast_free(head);
		context_free(ctx);
		exit(0);
	}
	else
		close(fds[1]);
	return (0);
}

static int16_t	_hd_rec_init(t_ast *node, t_sh_ctx *ctx, t_ast *head)
{
	if (!node)
		return (0);
	else if (node->type == ND_REDIR && node->u_data.rd.redir_type == RD_HEREDOC)
	{
		if (_hd_process(&node->u_data.rd, ctx, head))
			return (1);
		return (_hd_rec_init(node->u_data.rd.child, ctx, head));
	}
	else if (node->type == ND_REDIR)
		return (_hd_rec_init(node->u_data.rd.child, ctx, head));
	else if (node->type == ND_PIPE || node->type == ND_AND || node->type == ND_OR)
	{
		if (_hd_rec_init(node->u_data.op.left, ctx, head))
			return (1);
		return (_hd_rec_init(node->u_data.op.right, ctx, head));
	}
	else if (node->type == ND_SUBSHELL)
		return (_hd_rec_init(node->u_data.subsh.child, ctx, head));
	return (0);
}

int16_t	hd_init(t_ast *head, t_sh_ctx *ctx)
{
	int16_t	ret;
	int32_t status;
	size_t	count;

	ret = _hd_rec_init(head, ctx, head);
	count = exec_wait_get_count(head, true);
	while (count-- > 0)
	{
		if (wait(&status) == -1)
		{
			perror("minishell: hd_init: wait");
			continue ;
		}
		if (WIFSIGNALED(status))
			ctx->lst_exit = 128 + WTERMSIG(status);
	}
	return (ret);
}
