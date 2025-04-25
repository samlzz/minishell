/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_edit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:11:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/08 13:50:56 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lst.h"
#include <stdlib.h>

void	ft_lstadd_front(t_list **lst, t_list *new_node)
{
	if (new_node)
	{
		new_node->next = *lst;
		*lst = new_node;
	}
}

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*last;

	if (!*lst)
		return (ft_lstadd_front(lst, new_node));
	last = ft_lstlast(*lst);
	last->next = new_node;
}

int	ft_lstrm_byctn(t_list **lst, void *content, t_lst_f del)
{
	t_list	*curr;
	t_list	*prev;

	if (!*lst || !content || !del)
		return (2);
	prev = NULL;
	curr = *lst;
	while (curr && curr->content != content)
	{
		prev = curr;
		curr = curr->next;
	}
	if (!curr)
		return (1);
	if (prev)
		prev->next = curr->next;
	else
		*lst = curr->next;
	ft_lstdelone(curr, del);
	return (0);
}

void	ft_lstdelone(t_list *lst, t_lst_f del)
{
	if (!lst)
		return ;
	if (del)
		del(lst->content);
	free(lst);
}

void	ft_lstclear(t_list **lst, t_lst_f del)
{
	t_list	*curr;
	t_list	*nxt_save;

	curr = *lst;
	nxt_save = NULL;
	while (curr)
	{
		nxt_save = curr->next;
		ft_lstdelone(curr, del);
		curr = nxt_save;
		if (curr == *lst)
			break ;
	}
	*lst = NULL;
}
