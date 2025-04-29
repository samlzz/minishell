/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:36:25 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/28 21:55:06 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ast.h"
#include "env.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <readline/readline.h>
#include <stdlib.h>

static void	_gen_rd_filename(char *dest)
{
	static int	fallback_counter;
	int			fd;
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
}

static inline t_token	*_init_expand_vars(t_hmap *env, t_dynbuf *dest, \
	char *line)
{
	t_token	*tokens;
	t_token	*expanded;

	if (!dest || !line || !ft_strchr(line, '$'))
		return (NULL);
	*dest = ft_dynbuf_new(ft_strlen(line));
	if (!dest->data)
		return (NULL);
	tokens = hd_tokenise(line);
	if (!tokens)
		return (ft_dynbuf_free(dest), NULL);
	expanded = expand_token_list(env, tokens);
	token_clear(tokens);
	if (!expanded)
		return (ft_dynbuf_free(dest), NULL);
	return (expanded);
}

/**
 * @brief Expands a line using tokenization + expansion and reconstructs it.
 * 
 * @param env Environment map.
 * @param line Address of the line to expand (will be freed and replaced).
 */
static void _expand_line(t_hmap *env, char **line)
{
	t_token		*expanded;
	t_token		*cur;
	t_dynbuf	dest;

	expanded = _init_expand_vars(env, &dest, *line);
	if (!expanded)
		return ;
	cur = expanded;
	while (cur && cur->value)
	{
		if (!ft_dynbuf_append_str(&dest, cur->value))
			return (ft_dynbuf_free(&dest), token_clear(expanded));
		cur = cur->next;
	}
	token_clear(expanded);
	free(*line);
	*line = dest.data;
}

static inline int16_t	_create_heredoc(t_hmap *env, t_redir *redir)
{
	char	filename[PATH_MAX];
	int		fd;
	char	*line;

	_gen_rd_filename(filename);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (perror("minishell: open"), 1);
	while (1)
	{
		line = readline(HD_PROMPT);
		if (!line || ft_strcmp(line, redir->filename) == 0)
		{
			free(line);
			break;
		}
		if (redir->hd_expand)
			_expand_line(env, &line);
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	free(redir->filename);
	redir->filename = ft_strdup(filename);
	return (0);
}

int16_t	handle_heredocs(t_hmap *env, t_ast *node)
{
	if (!node)
		return (0);
	else if (node->type == ND_REDIR && node->u_data.rd.redir_type == RD_HEREDOC)
	{
		if (handle_heredocs(env, node->u_data.rd.child))
			return (1);
		return (_create_heredoc(env, &node->u_data.rd));
	}
	else if (node->type == ND_PIPE || node->type == ND_AND || node->type == ND_OR)
	{
		if (handle_heredocs(env, node->u_data.op.left))
			return (1);
		return (handle_heredocs(env, node->u_data.op.right));
	}
	else if (node->type == ND_SUBSHELL)
		return (handle_heredocs(env, node->u_data.s_subsh.child));
	return (0);
}
