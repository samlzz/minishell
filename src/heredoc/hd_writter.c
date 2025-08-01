/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_writter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:08:12 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/27 09:22:27 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>

#include "libft.h"
#include "here_doc.h"
#include "utils.h"
#include "env/env.h"
#include "handler/handler.h"

static inline void	_hd_read(int write_fd, char const *delim)
{
	char	*line;

	while (1)
	{
		line = ft_getinput(HD_PROMPT, false);
		if (!line || g_sig == 2 || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, write_fd);
		free(line);
	}
}

uint8_t	hd_process(t_redir *hd)
{
	int		fds[2];
	uint8_t	ret;

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
	ret = !!g_sig * (g_sig + 128);
	g_sig = 0;
	return (ret);
}

int	check_rl_done_before_user_entry(void)
{
	return (0);
}

uint8_t	hd_init(t_ast *head)
{
	int16_t	ret;

	sig_init(SIGH_HD);
	rl_event_hook = &check_rl_done_before_user_entry;
	ret = hd_rec_init(head);
	rl_event_hook = NULL;
	sig_init(SIGH_MAIN);
	return (ret);
}
