/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getdelim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 07:09:14 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/24 07:19:46 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "ft_gnl.h"
#include "ft_lst.h"
#include "gnl_private.h"
#include "libft.h"

#define BUFFER_SIZE 4096

static bool	_append_delim(size_t *len, char **res, char d, t_getdelim_file *f)
{
	size_t	i;
	size_t	end;
	char	*tmp;
	char	*cur;
	char	*n;

	n = ft_memchr(f->buf, d, f->len);
	end = !!n * (n - f->buf + 1) + !n * f->len;
	tmp = ft_calloc(*len + end + 1, 1);
	if (!tmp)
		return (free(*res), free(f->buf), *res = NULL, f->buf = NULL, true);
	cur = tmp;
	i = 0;
	if (*res)
	{
		while ((*res)[i])
			*cur++ = (*res)[i++];
		free(*res);
	}
	i = 0;
	while (i < end)
		*cur++ = f->buf[i++];
	*cur = 0;
	ft_memmove(f->buf, f->buf + end, f->len - end);
	return (f->len -= end, *res = tmp, *len += end, n != NULL);
}

static size_t	_parse_next_delim(char **r, char delim, t_getdelim_file *f)
{
	ssize_t	nbytes;
	size_t	l;

	l = 0;
	*r = NULL;
	if (!f->buf)
	{
		f->buf = ft_calloc(BUFFER_SIZE, sizeof(char));
		if (!f->buf)
			return (0);
		f->len = 0;
	}
	while (1)
	{
		if (f->len > 0 && _append_delim(&l, r, delim, f))
			return (free((void *)(!*r * ((uintptr_t)(f->buf)))),
				f->buf = (char *)(!!*r * (uintptr_t)f->buf), !!*r * l);
		nbytes = read(f->fd, f->buf, BUFFER_SIZE);
		if (nbytes < 0)
			return (free(f->buf), free(r), f->buf = NULL, r = NULL, 0);
		f->len = (size_t)nbytes;
		if (nbytes == 0)
			return (free(f->buf), f->buf = NULL, l);
	}
}

static t_list	*_get_getdelim_file(int fd, t_list *files)
{
	t_list	*prev;
	t_list	*cur;

	prev = NULL;
	cur = files;
	while (cur && cur->content && ((t_getdelim_file *) cur->content)->fd != fd)
	{
		prev = cur;
		cur = cur->next;
	}
	if (!cur)
	{
		cur = ft_calloc(1, sizeof(t_list));
		if (!cur)
			return (NULL);
		cur->content = ft_calloc(1, sizeof(t_getdelim_file));
		if (!cur->content)
			return (ft_lstdelone(cur, free), NULL);
		((t_getdelim_file *) cur->content)->fd = fd;
	}
	else if (prev)
		prev->next = cur->next;
	if (cur != files)
		cur->next = files;
	return (cur);
}

__attribute__((destructor))
static void	_ft_getdelim_destroy(void)
{
	t_list	*prev;
	t_list	*cur;

	ft_getdelim((char **) &cur, 0, -2147483648);
	while (cur)
	{
		prev = cur;
		cur = prev->next;
		if (prev->content)
			free(((t_getdelim_file *) prev->content)->buf);
		free(prev->content);
		free(prev);
	}
}

size_t	ft_getdelim(char **res, char delim, int fd)
{
	static t_list	*files = NULL;
	t_list			*f;
	size_t			len;

	if (fd == -2147483648)
		return (*res = (char *) files, 0);
	*res = NULL;
	f = _get_getdelim_file(fd, files);
	if (!f || !f->content)
		return (ft_lstdelone(f, free), 0);
	len = _parse_next_delim(res, delim, f->content);
	if (!*res)
	{
		if (f == files)
			files = files->next;
		return (ft_lstdelone(f, free), 0);
	}
	files = f;
	return (len);
}
