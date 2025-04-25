/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hmap_iter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 13:49:30 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/11 14:06:43 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hmap.h"

void	ft_hmap_iter(t_hmap *map, t_hm_f f)
{
	size_t	i;

	i = 0;
	while (i < map->__cap)
	{
		if (map->__entries[i].val)
			f(map->__entries[i].key, map->__entries[i].val);
		i++;
	}
}

void	ft_hmap_iter_full(t_hmap *map, t_hmd_f f, void *data)
{
	size_t	i;

	i = 0;
	while (i < map->__cap)
	{
		if (map->__entries[i].key || map->__entries[i].val)
			f(&map->__entries[i], data);
		i++;
	}
}
