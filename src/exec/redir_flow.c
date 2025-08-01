/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_flow.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:54:02 by mle-flem          #+#    #+#             */
/*   Updated: 2025/07/25 10:59:27 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libft.h"
#include "ft_gnl.h"
#include "exec.h"
#include "error/error.h"
#include "expansion/expander.h"
#include "heredoc/here_doc.h"

static void	_open_err(char *filename)
{
	char	*str;
	size_t	len;
	int32_t	errno_;

	errno_ = errno;
	len = ft_strlen("minishell: ") + ft_strlen(filename) + 1;
	str = malloc(len);
	if (!str)
		return (perror("minishell: malloc"));
	str[0] = 0;
	ft_strlcat(str, "minishell: ", len);
	ft_strlcat(str, filename, len);
	errno = errno_;
	perror(str);
	free(str);
}

static bool	_rd_in(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	fds[0] = open(node->u_data.rd.filename.expanded, O_RDONLY);
	if (fds[0] == -1)
		return (_open_err(node->u_data.rd.filename.expanded),
			rd_exit_clean(1, ctx, node, fds), false);
	return (true);
}

static bool	_rd_out_append(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	int32_t	oflags;

	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	oflags = O_WRONLY | O_CREAT | O_TRUNC;
	if (node->u_data.rd.redir_type == RD_APPEND)
		oflags = O_WRONLY | O_CREAT | O_APPEND;
	fds[1] = open(node->u_data.rd.filename.expanded, oflags, 0644);
	if (fds[1] == -1)
		return (_open_err(node->u_data.rd.filename.expanded),
			rd_exit_clean(1, ctx, node, fds), false);
	return (true);
}

static bool	_rd_hd_ex(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	char	*line;
	char	filename[PATH_MAX];
	int32_t	fd;

	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
	fd = hd_gen_filename(filename);
	if (fd == -1)
		return (_open_err(filename), rd_exit_clean(1, ctx, node, fds), false);
	fds[0] = open(filename, O_RDONLY);
	if (fds[0] == -1)
		return (_open_err(filename), close(fd),
			rd_exit_clean(1, ctx, node, fds), false);
	unlink(filename);
	while (ft_getline(&line, node->u_data.rd.fd))
	{
		hd_expand_line(ctx, &line);
		if (!line)
			return (perror("minishell: hd_expand_line"), close(fd),
				close(fds[0]), rd_exit_clean(1, ctx, node, fds), false);
		write(fd, line, ft_strlen(line));
	}
	free(line);
	close(fd);
	return (true);
}

void	exec_flow_redir(t_sh_ctx *ctx, t_ast *node, int32_t fds[2])
{
	t_token	*errtok;

	errtok = NULL;
	if (expand_node(ctx, node, &errtok))
		return (err_print_expand(errtok), rd_exit_clean(1, ctx, node, fds));
	if (node->u_data.rd.redir_type == RD_IN && !_rd_in(ctx, node, fds))
		return ;
	else if ((node->u_data.rd.redir_type == RD_OUT
			|| node->u_data.rd.redir_type == RD_APPEND)
		&& !_rd_out_append(ctx, node, fds))
		return ;
	else if (node->u_data.rd.redir_type == RD_HEREDOC
		&& node->u_data.rd.hd_expand && !_rd_hd_ex(ctx, node, fds))
		return ;
	else if (node->u_data.rd.redir_type == RD_HEREDOC
		&& !node->u_data.rd.hd_expand)
	{
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = node->u_data.rd.fd;
	}
	if (node->u_data.rd.child)
		exec_flow_cmd(ctx, node->u_data.rd.child, fds);
	else
		return (rd_exit_clean(0, ctx, node, fds));
}
