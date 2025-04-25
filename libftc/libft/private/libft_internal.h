/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:34:46 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/11 13:23:18 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_INTERNAL_H
# define LIBFT_INTERNAL_H

# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>

/**
 * @struct s_mem
 * @brief Represents a memory table with content and size.
 */
typedef struct s_mem
{
	char	*content;
	size_t	size;
}	t_mem;

//* conversion utils
void	_nb_to_asciistr(int64_t nb, t_mem *dest, char *base, size_t b_len);
t_mem	*_init_dest_to_convert(int64_t n, size_t base_l);

bool	ft_valid_base(char *base, size_t *len);

#endif
