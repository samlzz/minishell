/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_expander.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:50:04 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 12:39:04 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <stdlib.h>

/**
 * @brief Convert an argword to a single token.
 * 
 * It optionally save his unexpanded value.
 *
 * @param word The argument word structure.
 * @param raw The original unexpanded string (optional).
 * @return t_token* A new token created from the argword, or NULL on error.
 * 
 * @note `quote` is ignored bc we still only need it for here_docs(which are 
 * handled separately) since expansions, joints and splitted have been done.
 */
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

/**
 * @brief Convert a linked list of argwords into a linked list of tokens.
 *
 * @param words The list of argwords.
 * @param og Original token used to copy the unexpanded form.
 * @return t_token* List of tokens, or NULL on error.
 */
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

/**
 * @brief Expand a single token into a list of tokens (handling quotes, splitting, wildcards).
 *
 * In order:
 * 1) expand the variables
 * 2) merge the words not separated by spaces
 * 3) split words with spaces in the middle
 * 4) expand the wildcards.

 * @param env The environment hashmap.
 * @param cur Pointer to the current token pointer (will be advanced).
 * @param prev The previous token (for redirection detection).
 * @return t_token* Resulting expanded token list or NULL on error.
 */
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

/**
 * @brief Expand heredoc delimiter by merging glued tokens and preserving quotes.
 *
 * @param delim The heredoc delimiter token.
 * @param cur The current glued token to merge.
 * @return t_token* Updated delimiter token, or NULL on error.
 * 
 * @note If current value is "$" and next token is quoted ;
 *       empty expand current.
 */
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

/**
 * @brief Expand a heredoc token and glue all glued TK_WORD tokens to it.
 *
 * @param lst Pointer to the current token pointer (will be advanced).
 * @return t_token* The expanded heredoc token pair, or NULL on error.
 */
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

/**
 * @brief Expand a full token list: 
 * handle variables, heredocs, splitting, wildcards.
 *
 * @param env Environment map.
 * @param lst Original token list to expand.
 * @return t_token* New list of expanded tokens, or NULL on error.
 */
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
