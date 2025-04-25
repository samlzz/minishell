/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 15:22:04 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/08 22:11:10 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

int16_t	ft_isalpha(int32_t c)
{
	return ((c >= 'A' && c <= 'Z') || \
		(c >= 'a' && c <= 'z'));
}

int16_t	ft_isdigit(int32_t c)
{
	return (c >= '0' && c <= '9');
}

int16_t	ft_isalnum(int32_t c)
{
	return (ft_isalpha(c) || ft_isdigit(c));
}

int16_t	ft_isascii(int32_t c)
{
	return (c >= 0 && c <= 127);
}

int16_t	ft_isprint(int32_t c)
{
	return (c >= 32 && c <= 126);
}
