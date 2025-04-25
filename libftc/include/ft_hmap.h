/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hmap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 12:18:20 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/21 17:43:38 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HMAP_H
# define FT_HMAP_H

# include "libft.h"

# ifndef HMAP_INIT_CAP
#  define HMAP_INIT_CAP 16
# endif

# ifndef HMAP_RESIZE_RATIO
#  define HMAP_RESIZE_RATIO 0.7f
# endif

// constants (64-bit FNV-1a standard):
# define FNV_OFFSET_BASIC	14695981039346656037ULL
# define FNV_PRIME			1099511628211ULL

typedef struct s_hmap_entry
{
	char	*key;
	void	*val;
}	t_hm_entry;

typedef struct s_hmap
{
	size_t		count;
	t_hm_entry	*__entries;
	size_t		__cap;
}	t_hmap;

// Hmap
t_hmap		ft_hmap_new(size_t *init_cap);
void		ft_hmap_free(t_hmap *map, void (*del)(void *));

typedef char *	(*t_concat_entry) (char *, void *);
char		**ft_hmap_to_array(t_hmap *hmap, t_concat_entry f);

int16_t		ft_hmap_set(t_hmap *map, const char *key, void *value, \
			void (*del)(void *));
void		*ft_hmap_get(t_hmap *map, const char *key);
int16_t		ft_hmap_rm(t_hmap *map, const char *key, void (*del)(void *));

// iter
typedef void	(*t_hm_f) (char *, void *);
typedef void	(*t_hmd_f) (t_hm_entry *, void *);

void		ft_hmap_iter(t_hmap *map, t_hm_f f);
void		ft_hmap_iter_full(t_hmap *map, t_hmd_f func, void *data);

#endif
