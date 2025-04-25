/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 13:47:23 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/11 14:07:54 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LST_H
# define FT_LST_H

# ifndef LIBFT_H
#  include <stddef.h>
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef void	(*t_lst_f) (void *);

//* Functions

// get
t_list			*ft_lstnew(void *content);

int				ft_lstsize(t_list *lst) __attribute__ ((pure));

t_list			*ft_lstlast(t_list *lst)__attribute__ ((warn_unused_result));

t_list			*ft_lstoflast(t_list *lst, size_t offset)
				__attribute__((deprecated("This func needs to be re-written")));

// edit
void			ft_lstadd_front(t_list **lst, t_list *new_node)
				__attribute__ ((nonnull(1)));

void			ft_lstadd_back(t_list **lst, t_list *new_node)
				__attribute__ ((nonnull(1)));

int				ft_lstrm_byctn(t_list **lst, void *content, t_lst_f del)
				__attribute__ ((nonnull(1)));

void			ft_lstdelone(t_list *lst, t_lst_f del);

void			ft_lstclear(t_list **lst, t_lst_f del)
				__attribute__ ((nonnull(1)));

// iter
void			ft_lstiter(t_list *lst, t_lst_f f)
				__attribute__ ((nonnull(2)));

void			ft_lstiteri(t_list *lst, void (*f)(void *, size_t))
				__attribute__ ((nonnull(2)));

t_list			*ft_lstmap(t_list *lst, void *(*f)(void *), t_lst_f del)
				__attribute__ ((warn_unused_result, nonnull(2)));

#endif
