/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_output.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:26:27 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/11 17:43:30 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

#define PUTNBR_BASE 10

void	ft_putchar_fd(char c, int32_t fd)
{
	if (fd < 0)
		return ;
	write(fd, &c, 1);
}

void	ft_putstr_fd(const char *s, int32_t fd)
{
	if (fd < 0)
		return ;
	write(fd, s, ft_strlen(s));
}

void	ft_putendl_fd(const char *s, int32_t fd)
{
	if (fd < 0)
		return ;
	ft_putstr_fd(s, fd);
	write(fd, "\n", 1);
}

static void	_recurs_putnbr_fd(int64_t n, int32_t fd)
{
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n = -n;
	}
	if (n >= PUTNBR_BASE)
		_recurs_putnbr_fd(n / PUTNBR_BASE, fd);
	ft_putchar_fd((n % PUTNBR_BASE) + '0', fd);
}

void	ft_putnbr_fd(int32_t n, int32_t fd)
{
	if (fd < 0)
		return ;
	_recurs_putnbr_fd((int64_t)n, fd);
}
