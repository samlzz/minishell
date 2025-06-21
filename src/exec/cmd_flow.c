/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_flow.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 18:54:02 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/21 11:37:42 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "exec/exec.h"
#include "heredoc/here_doc.h"

static void	_dup_err(int32_t oldfd, int32_t newfd)
{
	char str[61];

	str[0] = 0;
	ft_strlcat(str, "minishell: cannot duplicate fd ", 61);
	ft_itoa_str(str + ft_strlen(str), oldfd);
	ft_strlcat(str, " to fd ", 61);
	ft_itoa_str(str + ft_strlen(str), newfd);
	perror(str);
}

static void	_open_err(char *filename)
{
	char	str[12 + NAME_MAX];

	str[0] = 0;
	ft_strlcat(str, "minishell: ", 12 + NAME_MAX);
	ft_strlcat(str, filename, 12 + NAME_MAX);
	perror(str);
	exit(1);
}

static void _dup_fds(int32_t fds[2])
{
	if (dup2(fds[1], STDOUT_FILENO) == -1)
		_dup_err(fds[1], STDOUT_FILENO);
	if (fds[1] != STDOUT_FILENO)
		close(fds[1]);
	if (dup2(fds[0], STDIN_FILENO) == -1)
		_dup_err(fds[0], STDIN_FILENO);
	if (fds[0] != STDIN_FILENO)
		close(fds[0]);
}

static void	_exec_flow_cmd_redir(t_hmap *env, t_ast *root, t_ast *node, int32_t fds[2])
{
	t_token	*errtok;
	char	filename[PATH_MAX];
	char	*line;
	FILE	*stream;
	size_t	n;
	int32_t	oflags;
	int32_t	fd;

	errtok = NULL;
	if (expand_node(node, env, &errtok))
		return (ast_free(root), ft_hmap_free(env, free), err_print_expand(errtok));
	if (node->u_data.rd.redir_type == RD_IN)
	{
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = open(node->u_data.rd.filename.expanded, O_RDONLY);
		if (fds[0] == -1)
			return (_open_err(node->u_data.rd.filename.expanded));
	}
	else if (node->u_data.rd.redir_type == RD_OUT
		|| node->u_data.rd.redir_type == RD_APPEND)
	{
		if (fds[1] != STDOUT_FILENO)
			close(fds[1]);
		oflags = O_WRONLY | O_CREAT | O_TRUNC;
		if (node->u_data.rd.redir_type == RD_APPEND)
			oflags = O_WRONLY | O_CREAT | O_APPEND;
		fds[1] = open(node->u_data.rd.filename.expanded, oflags, 0644);
		if (fds[1] == -1)
			return (_open_err(node->u_data.rd.filename.expanded));
	}
	else if (node->u_data.rd.redir_type == RD_HEREDOC && node->u_data.rd.hd_expand)
	{
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fd = gen_heredoc_filename(filename);
		if (fd == -1)
			return (_open_err(filename));
		fds[0] = open(filename, O_RDONLY);
		if (fds[0] == -1)
			return (_open_err(filename), (void) close(fd));
		unlink(filename);
		line = NULL;
		n = 0;
		stream = fdopen(node->u_data.rd.fd, "r");
		while (getline(&line, &n, stream) != -1) // FIXME: use ft_getline
		{
			expand_line(env, &line);
			if (!line)
				return (perror("minishell: expand_line"));
			write(fd, line, ft_strlen(line));
		}
		free(line);
		fclose(stream);
		close(fd);
	}
	else if (node->u_data.rd.redir_type == RD_HEREDOC && !node->u_data.rd.hd_expand)
	{
		if (fds[0] != STDIN_FILENO)
			close(fds[0]);
		fds[0] = node->u_data.rd.fd;
	}
	if (node->u_data.rd.child)
		exec_flow_cmd(env, root, node->u_data.rd.child, fds);
	else
		exit(0);
}

static void	_exec_flow_cmd_cmd(t_hmap *env, t_ast *root, t_ast *node, int32_t fds[2])
{
	char	**argv;
	char	**envp;
	t_token	*errtok;
	char	*cmd;

	errtok = NULL;
	if (expand_node(node, env, &errtok))
		return (err_print_expand(errtok));
	argv = &node->u_data.cmd.args->expanded;
	node->u_data.cmd.args = NULL;
	ast_free(root);
	envp = get_envp(env);
	if (!envp)
		return (perror("cannot get envp"));
	cmd = exec_get_cmd_path(argv, env);
	if (access(cmd, F_OK))
		return (perror("minishell: command not found"), free(cmd), ft_splitfree(argv, 0), ft_hmap_free(env, free), exit(127));
	if (access(cmd, X_OK))
		return (perror("minishell: command not executable"), free(cmd), ft_splitfree(argv, 0), ft_hmap_free(env, free), exit(126));
	_dup_fds(fds);
	execve(cmd, argv, envp);
}

void	exec_flow_cmd(t_hmap *env, t_ast *root, t_ast *node, int32_t fds[2])
{
	if (node && node->type == ND_REDIR)
		_exec_flow_cmd_redir(env, root, node, fds);
	else if (node && node->type == ND_CMD)
		_exec_flow_cmd_cmd(env, root, node, fds);
}
