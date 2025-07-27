/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 16:13:30 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/27 09:22:15 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>

#include "libft.h"
#include "ft_gnl.h"
#include "utils.h"

char	*ft_getinput(char const *prompt, bool is_main)
{
	size_t	len;
	char	*input;

	if (isatty(STDIN_FILENO))
	{
		rl_outstream = stderr;
		input = readline(prompt);
		if (!input && is_main)
			return (ft_putendl_fd("exit", STDERR_FILENO), NULL);
		else if (!input)
			return (NULL);
	}
	else
	{
		ft_getline(&input, STDIN_FILENO);
		if (!input)
			return (NULL);
		len = ft_strlen(input) - 1;
		if (input[len] == '\n')
			input[len] = '\0';
	}
	return (input);
}

bool	ft_isspace(int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	return (
		uc == ' ' || uc == '\f' \
		|| uc == '\n' || uc == '\r' \
		|| uc == '\t' || uc == '\v'
	);
}

char	*ft_str3join(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*ret;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	ret = ft_strjoin(tmp, s3);
	free(tmp);
	return (ret);
}

bool	ft_strjreplace(char **target, const char *suff)
{
	char	*tmp;

	tmp = ft_strappend(*target, suff);
	if (!tmp)
		return (true);
	free(*target);
	*target = tmp;
	return (false);
}
