/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:50:04 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/22 18:39:42 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

static inline t_token	*_tokens_from_argwords(t_argword *word)
{
	t_argword	*lst;
	t_token		*dest;
	t_token		*new;

	lst = word;
	dest = NULL;
	while (word)
	{
		new = ft_calloc(1, sizeof (t_token));
		if (!new)
			return (argword_clear(lst), \
					token_clear(dest), NULL);
		new->type = TK_WORD;
		new->value = word->value;
		if (word->no_quote)
			new->quote = QUOTE_NONE;
		else
			new->quote = QUOTE_SINGLE;
		word->value = NULL;
		token_addback(&dest, new);
		word = word->next;
	}
	argword_clear(lst);
	return (dest);
}

static t_token	*_expand_one_tk(t_hmap *env, t_token **cur)
{
	t_argword	*new;
	t_argword	*tmp;

	new = build_argword(env, cur, 0);
	if (!new)
		return (NULL);
	if (new->space_offsets.len)
	{
		tmp = split_withespace(new);
		argword_clear(new);
		if (!tmp)
			return (NULL);
		new = tmp;
	}
	new = expand_wildcards_in_list(new);
	return (_tokens_from_argwords(new));
}

t_token	*expand_token_list(t_hmap *env, t_token *lst, t_token **errtok)
{
	t_token	*new_lst;
	t_token	*node;

	new_lst = NULL;
	while (lst && lst->type != TK_EOF)
	{
		if (lst->type == TK_WORD)
			node = _expand_one_tk(env, &lst);
		else
		{
			node = token_dup(lst);
			lst = lst->next;
		}
		if (!node)
			return (token_clear(new_lst), NULL);
		token_addback(&new_lst, node);
	}
	if (!new_lst || !lst)
		*errtok = lst;
	return (new_lst);
}
