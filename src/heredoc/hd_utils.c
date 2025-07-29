/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:11:06 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/29 08:39:34 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "libft.h"
#include "here_doc.h"
#include "utils.h"
#include "expansion/expander.h"

/**
 * @brief Generate a unique temporary heredoc filename.
 *
 * Tries to use `/dev/urandom`, falls back to a counter if not available.
 *
 * @param dest Output buffer to write the generated filename into.
 * @return write fd of the unique file that was generated
 */
int	hd_gen_filename(char *dest)
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
	return (open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0600));
}

int32_t	ft_pipe(int fds[2])
{
	char	tmpfile[PATH_MAX];

	fds[1] = hd_gen_filename(tmpfile);
	if (fds[1] == -1)
		return (-1);
	fds[0] = open(tmpfile, O_RDONLY);
	if (fds[0] == -1)
	{
		close(fds[1]);
		return (-1);
	}
	unlink(tmpfile);
	return (0);
}

void	hd_expand_line(t_sh_ctx *ctx, char **line)
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
	while (cur && cur->type != TK_EOF)
	{
		expanded = expand_word_handler(ctx, &cur, false, false);
		if (!expanded || !ft_dynbuf_append_str(&dest, expanded->value))
			return (token_clear(tokens), ft_dynbuf_free(&dest));
	}
	free(*line);
	*line = dest.data;
}

int16_t	hd_quotes_removing(t_redir *node)
{
	char	*filename;
	t_token	*cur;

	filename = NULL;
	cur = node->filename.tk;
	while (cur)
	{
		if (ft_strjreplace(&filename, cur->value))
			return (perror("minishell: hd_quotes_removing: malloc"), \
				free(filename), 1);
		cur = cur->next;
		if (!cur || !cur->glued)
			break ;
	}
	token_clear(node->filename.tk);
	node->filename.expanded = filename;
	node->is_expanded = true;
	return (0);
}
