/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_writer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:03:58 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/10 12:03:45 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"
#include "env/env.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

void	expand_line(t_hmap *env, char **line)
{
	t_token		*tokens;
	t_token		*cur;
	t_argword	*expanded;
	t_dynbuf	dest;

	if (!*line || !ft_strchr(*line, '$'))
		return ;
	dest = ft_dynbuf_new(ft_strlen(*line));
	if (!dest.data)
		return ;
	tokens = hd_tokenise(*line);
	if (!tokens)
		return (ft_dynbuf_free(&dest));
	cur = tokens;
	while (cur)
	{
		expanded = expand_word(&cur, false, env);
		if (!expanded || !ft_dynbuf_append_str(&dest, expanded->value))
			return (token_clear(tokens), ft_dynbuf_free(&dest));
	}
	free(*line);
	*line = dest.data;
}

/**
 * @brief Generate a unique temporary heredoc filename.
 *
 * Tries to use `/dev/urandom`, falls back to a counter if not available.
 *
 * @param dest Output buffer to write the generated filename into.
 * @return fd of the unique file that was generated
 */
static int32_t	_gen_rd_filename(char *dest)
{
	static int	fallback_counter;
	int32_t		fd;
	uint8_t		buf[8];
	int32_t		i;
	
	ft_memmove(dest, HD_FNAME_BASE, HD_FN_LEN);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0 || read(fd, buf, sizeof(buf)) != sizeof(buf))
	{
		i = 0;
		while (i < 8)
		{
			buf[i] = (fallback_counter + i) & 0xFF;
			i++;
		}
		fallback_counter++;
	}
	if (fd >= 0)
		close(fd);
	i = -1;
	while (++i < 8)
		dest[HD_FN_LEN + i] = HD_CHARSET[buf[i] % HD_CHARSET_LEN];
	dest[HD_FN_LEN + i] = '\0';
	return (open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0600));
}

/**
 * @brief Create and write the heredoc file.
 *
 * Reads from stdin until a delimiter match is found.
 *
 * @param redir Pointer to the heredoc redirection node.
 * @return int16_t 0 on success, 1 on failure.
 */
static inline int16_t	_create_heredoc(t_redir *redir)
{
	char	filename[PATH_MAX];
	int32_t fd;
	char	*line;

	fd = _gen_rd_filename(filename);
	if (fd == -1)
		return (perror("minishell: open"), 1);
	redir->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	unlink(filename);
	if (redir->fd == -1)
		return (perror("minishell: open"), close(fd), 1);
	while (1)
	{
		line = readline(HD_PROMPT);
		if (!line || ft_strcmp(line, redir->filename.expanded) == 0)
		{
			free(line);
			break;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (close(fd), 0);
}

/**
 * @brief Traverse the AST and handle all heredoc redirections recursively.
 *
 * Read user entry until limiter founded, then write to a temporary files.
 *
 * @param node AST node to inspect.
 * @return int16_t 0 on success, 1 if an error occurred.
 */
int16_t	write_heredocs(t_ast *node)
{
	if (!node)
		return (0);
	else if (node->type == ND_REDIR && node->u_data.rd.redir_type == RD_HEREDOC)
	{
		if (write_heredocs(node->u_data.rd.child))
			return (1);
		return (_create_heredoc(&node->u_data.rd));
	}
	else if (node->type == ND_REDIR)
		return (write_heredocs(node->u_data.rd.child));
	else if (node->type == ND_PIPE || node->type == ND_AND || node->type == ND_OR)
	{
		if (write_heredocs(node->u_data.op.left))
			return (1);
		return (write_heredocs(node->u_data.op.right));
	}
	else if (node->type == ND_SUBSHELL)
		return (write_heredocs(node->u_data.subsh.child));
	return (0);
}
