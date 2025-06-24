/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl_private.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mle-flem <mle-flem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 07:11:31 by mle-flem          #+#    #+#             */
/*   Updated: 2025/06/24 07:12:29 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GNL_PRIVATE_H
# define GNL_PRIVATE_H

# include <stddef.h>

typedef struct s_getdelim_file	t_getdelim_file;
struct s_getdelim_file
{
	int				fd;
	char			*buf;
	size_t			len;
	t_getdelim_file	*next;
};

#endif
