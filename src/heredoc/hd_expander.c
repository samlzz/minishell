/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:40:16 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/10 16:42:03 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "here_doc.h"

int16_t	expand_heredoc(t_ast *node)
{
	char	*filename;
	char	*tmp;
	t_token	*cur;
	
	filename = NULL;
	cur = node->u_data.rd.filename.tk;
	while (cur)
	{
		tmp = ft_strappend(filename, cur->value);
		if (!tmp)
			return (free(filename), 1);
		cur = cur->next;
		if (!cur || !cur->glued)
			break ;
	}
	token_clear(node->u_data.rd.filename.tk);
	node->u_data.rd.filename.expanded = filename;
	return (0);
}
