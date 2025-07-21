/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tild_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 20:52:09 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/22 21:17:59 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "libft.h"
#include "ft_gnl.h"

static char	*_extract_value(int fd, char *(*parse_line) (char **, char *),
	char *to_comp)
{
	char	**split;
	char	*line;
	char	*value;

	value = NULL;
	while (ft_getline(&line, fd))
	{
		split = ft_nsplit(line, ':');
		if (split)
		{
			value = parse_line(split, to_comp);
			free(split);
		}
		free(line);
		if (value)
			return (value);
	}
	return (NULL);
}

static char	*_get_uid_from_line(char **line, char *uid_str)
{
	if (line[2] && ft_strcmp(line[2], uid_str) == 0)
		return (ft_strdup(line[0]));
	return (NULL);
}

char	*ft_getuser(void)
{
	struct stat	sb;
	char		*uid_str;
	char		*user;
	int			fd;

	if (stat("/proc/self/stat", &sb) == -1)
		return (NULL);
	uid_str = ft_itoa(sb.st_uid);
	if (!uid_str)
		return (NULL);
	fd = open("/etc/passwd", O_RDONLY);
	if (fd == -1)
		return (free(uid_str), NULL);
	user = _extract_value(fd, &_get_uid_from_line, uid_str);
	close(fd);
	free(uid_str);
	return (user);
}

static char	*_get_home_from_line(char **line, char *username)
{
	if (line[0] && line[5] && ft_strcmp(line[0], username) == 0)
		return (ft_strdup(line[5]));
	return (NULL);
}

char	*ft_gethome(const char *username)
{
	int		fd;
	char	*home;

	if (!username)
		return (NULL);
	fd = open("/etc/passwd", O_RDONLY);
	if (fd == -1)
		return (NULL);
	home = _extract_value(fd, &_get_home_from_line, (char *)username);
	close(fd);
	return (home);
}
