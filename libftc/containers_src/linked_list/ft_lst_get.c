/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_get.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:11:52 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/08 13:50:35 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_lst.h"
#include <stdlib.h>

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = malloc(sizeof (t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

int	ft_lstsize(t_list *lst)
{
	int		len;
	t_list	*first;

	len = 0;
	if (!lst)
		return (len);
	first = lst;
	while (lst)
	{
		len++;
		lst = lst->next;
		if (first == lst)
			break ;
	}
	return (len);
}

t_list	*ft_lstoflast(t_list *lst, size_t offset)
{
	t_list	*lead;
	t_list	*follow;
	size_t	i;

	if (!lst)
		return (NULL);
	i = 0;
	lead = lst;
	follow = lst;
	while (i++ < offset)
	{
		lead = lead->next;
		if (!lead)
			return (NULL);
	}
	while (lead->next)
	{
		lead = lead->next;
		follow = follow->next;
	}
	return (follow);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
