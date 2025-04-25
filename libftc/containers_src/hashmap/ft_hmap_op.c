/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hmap_op.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 12:53:50 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/21 14:20:44 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "private/ft_hmap_internal.h"

int16_t	ft_hmap_set(t_hmap *map, const char *key, void *val, \
	void (*del)(void *))
{
	size_t		idx;
	t_hm_entry	new;
	char		*__key;

	idx = _get_good_idx(map, key);
	if (map->__entries[idx].val)
	{
		del(map->__entries[idx].val);
		map->__entries[idx].val = val;
		return (0);
	}
	__key = ft_strdup(key);
	if (!__key)
		return (1);
	map->count++;
	if (_hmap_resize(map))
		return (1);
	new = (t_hm_entry){__key, val};
	_fill_entry(&new, map);
	return (0);
}

void	*ft_hmap_get(t_hmap *map, const char *key)
{
	size_t	idx;

	idx = _get_good_idx(map, key);
	if (!map->__entries[idx].key)
		return (NULL);
	return (map->__entries[idx].val);
}

int16_t	ft_hmap_rm(t_hmap *map, const char *key, void (*del)(void *))
{
	size_t	idx;

	idx = _get_good_idx(map, key);
	if (!map->__entries[idx].key || !map->__entries[idx].val)
		return (1);
	del(map->__entries[idx].val);
	map->__entries[idx].val = NULL;
	map->count--;
	return (0);
}
