/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:18:34 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/21 17:43:26 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hmap.h"
#include "private/ft_hmap_internal.h"
#include <stdlib.h>

t_hmap	ft_hmap_new(size_t *init_cap)
{
	t_hmap	new;

	new.count = 0;
	if (init_cap)
		new.__cap = *init_cap;
	else
		new.__cap = HMAP_INIT_CAP;
	new.__entries = ft_calloc(new.__cap, sizeof (t_hm_entry));
	if (!new.__entries)
		new.__cap = 0;
	return (new);
}

static void	_destroy_entry(t_hm_entry *elem, void (*del)(void *))
{
	if (elem->val)
		del(elem->val);
	free(elem->key);
}

void	ft_hmap_free(t_hmap *map, void (*del)(void *))
{
	ft_hmap_iter_full(map, (t_hmd_f)_destroy_entry, (void *)del);
	free(map->__entries);
	map->__entries = NULL;
	map->count = 0;
	map->__cap = 0;
}

static void	_append_entry(t_hm_entry *entry, void *ctx_ptr)
{
	t_to_array_ctx	*ctx;
	char			*str;

	if (!entry->val)
		return ;
	ctx = (t_to_array_ctx *)ctx_ptr;
	str = ctx->f(entry->key, entry->val);
	if (!str)
	{
		ft_splitfree(ctx->array, ctx->i);
		ctx->array = NULL;
		return ;
	}
	ctx->array[ctx->i++] = str;
}

char	**ft_hmap_to_array(t_hmap *hmap, t_concat_entry join)
{
	char			**result;
	t_to_array_ctx	ctx;

	result = malloc((hmap->count + 1) * sizeof (char *));
	if (!result)
		return (NULL);
	ctx.array = result;
	ctx.i = 0;
	ctx.f = join;
	ft_hmap_iter_full(hmap, _append_entry, &ctx);
	if (!ctx.array)
		return (NULL);
	ctx.array[ctx.i] = NULL;
	return (result);
}
