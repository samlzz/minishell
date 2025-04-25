/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_iter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:11:04 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/08 13:50:47 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lst.h"

void	ft_lstiter(t_list *lst, t_lst_f f)
{
	t_list	*first;

	first = lst;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
		if (first == lst)
			break ;
	}
}

void	ft_lstiteri(t_list *lst, void (*f)(void *, size_t))
{
	t_list	*first;
	size_t	i;

	i = 0;
	first = lst;
	while (lst)
	{
		f(lst->content, i++);
		lst = lst->next;
		if (first == lst)
			break ;
	}
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), t_lst_f del)
{
	t_list	*first;
	t_list	*mapped;
	t_list	*new;
	void	*content;

	mapped = NULL;
	new = NULL;
	first = lst;
	while (lst)
	{
		content = f(lst->content);
		new = ft_lstnew(content);
		if (!new)
		{
			del(content);
			ft_lstclear(&mapped, del);
			return (NULL);
		}
		ft_lstadd_back(&mapped, new);
		lst = lst->next;
		if (lst == first)
			return (ft_lstclear(&mapped, del), NULL);
	}
	return (mapped);
}
