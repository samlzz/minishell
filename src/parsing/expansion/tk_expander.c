/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:50:04 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/29 13:41:09 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

static t_token	*_consume_argword(t_argword *word, char *raw)
{
	t_token	*new;
	
	new = ft_calloc(1, sizeof (t_token));
	if (!new)
		return (NULL);
	new->type = TK_WORD;
	if (raw && ft_strcmp(word->value, raw))
		new->unexpanded = raw;
	else
		free(raw);
	new->value = word->value;
	word->value = NULL;
	return (new);
}

static inline t_token	*_tokens_from_argwords(t_argword *words, t_token *og)
{
	t_argword	*cur;
	t_token		*dest;
	t_token		*new;
	char		*raw;

	cur = words;
	dest = NULL;
	while (cur)
	{
		raw = NULL;
		if (og && og->value)
			raw = ft_strdup(og->value);
		new = _consume_argword(cur, raw);
		if (!new)
			return (argword_clear(words), \
					token_clear(dest), NULL);
		token_addback(&dest, new);
		cur = cur->next;
	}
	argword_clear(words);
	return (dest);
}

static t_token	*_expand_one_tk(t_hmap *env, t_token **cur, t_token *prev)
{
	t_token		*og;
	t_argword	*argword;
	t_argword	*split;

	og = *cur;
	argword = build_argword(env, cur, 0);
	if (!argword)
		return (NULL);
	if (argword->space_offsets.len && (!prev || !is_redirection(prev->type)))
	{
		split = split_withespace(argword);
		argword_clear(argword);
		if (!split)
			return (NULL);
		argword = split;
	}
	argword = replace_by_wild_expanded(argword);
	return (_tokens_from_argwords(argword, og));
}

static inline t_token	*_fill_delim(t_token *delim, t_token *cur)
{
	char	*tmp;

	if (delim->quote == QUOTE_NONE && cur->quote != QUOTE_NONE \
		&& !ft_strncmp(delim->value, "$", 1))
	{
		tmp = ft_strdup(cur->value);
	}
	else
	{
		tmp = ft_strappend(delim->value, cur->value);
	}
	if (!tmp)
		return (NULL);
	free(delim->value);
	delim->value = tmp;
	if (cur->quote != QUOTE_NONE)
		delim->quote = QUOTE_SINGLE;
	return (delim);
}

static t_token	*_expand_heredoc(t_token **lst)
{
	t_token	*heredoc;
	t_token	*delim;

	heredoc = token_dup(*lst);
	next(lst);
	if (!heredoc || !*lst)
		return (token_clear(heredoc), NULL);
	delim = token_dup(*lst);
	next(lst);
	if (!delim)
		return (token_clear(heredoc), NULL);
	heredoc->next = delim;
	while (*lst && (*lst)->type == TK_WORD && (*lst)->glued)
	{
		if (!_fill_delim(delim, *lst))
			return (token_clear(heredoc), NULL);
		next(lst);
	}
	return (heredoc);
}

t_token	*expand_token_list(t_hmap *env, t_token *lst)
{
	t_token	*new_lst;
	t_token	*node;
	t_token	*prev;

	new_lst = NULL;
	prev = NULL;
	while (lst && lst->type != TK_EOF)
	{
		if (lst->type == TK_WORD)
			node = _expand_one_tk(env, &lst, prev);
		else if (lst->type == TK_HEREDOC)
			node = _expand_heredoc(&lst);
		else
		{
			prev = lst;
			node = token_dup(lst);
			lst = lst->next;
		}
		if (!node)
			return (token_clear(new_lst), NULL);
		token_addback(&new_lst, node);
	}
	return (new_lst);
}
