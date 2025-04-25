/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hmap_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:32:46 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/11 14:10:50 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hmap_internal.h"
#include <stdlib.h>

/**
 * @brief Computes the FNV-1a 64-bit hash of a null-terminated string.
 *
 * @param str The input string to hash (null-terminated).
 * @return uint64_t The resulting 64-bit hash value.
 *
 * @note The hash is calculated as follows:
 *	hash := FNV_offset_basis (64-bit)
 *
 *	for each byte_of_data in input:
 *
 *		- hash := hash XOR byte_of_data
 *
 *		- hash := hash × FNV_prime (mod 2^64)
 */
static inline uint64_t	fnv1a_hash(const char *key)
{
	uint64_t	hash;

	hash = FNV_OFFSET_BASIC;
	while (*key)
	{
		hash ^= (uint8_t)(*key++);
		hash *= FNV_PRIME;
	}
	return (hash);
}

void	_fill_entry(t_hm_entry *og, t_hmap *map)
{
	size_t	idx;

	idx = fnv1a_hash(og->key) % map->__cap;
	if (map->__entries[idx].val)
	{
		while (map->__entries[idx].val)
		{
			if (!map->__entries[idx].key)
				return ;
			idx = (idx + 1) % map->__cap;
		}
	}
	if (map->__entries[idx].key)
		free(map->__entries[idx].key);
	map->__entries[idx].key = og->key;
	map->__entries[idx].val = og->val;
}

static inline bool	hmap_should_resize(t_hmap map)
{
	return (((float)map.count / (float)map.__cap) > HMAP_RESIZE_RATIO);
}

int16_t	_hmap_resize(t_hmap *map)
{
	t_hmap	new_map;

	if (!hmap_should_resize(*map))
		return (0);
	new_map.__cap = map->__cap * 2;
	if (new_map.__cap < map->__cap || new_map.__cap >= INT64_MAX - 8)
		return (1);
	new_map.__entries = ft_calloc(new_map.__cap, sizeof (t_hm_entry));
	if (!new_map.__entries)
		return (1);
	new_map.count = map->count;
	ft_hmap_iter_full(map, (t_hmd_f)_fill_entry, &new_map);
	free(map->__entries);
	*map = new_map;
	return (0);
}

size_t	_get_good_idx(t_hmap *map, const char *key)
{
	size_t	idx;

	idx = fnv1a_hash(key) % map->__cap;
	while (map->__entries[idx].key && ft_strcmp(key, map->__entries[idx].key))
	{
		idx = (idx + 1) % map->__cap;
	}
	return (idx);
}
