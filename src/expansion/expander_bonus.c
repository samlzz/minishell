/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:21:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/28 16:18:31 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef MINISHELL_BONUS

# include "expander.h"
# include "lexer/token.h"
# include "libft.h"
# include <stdbool.h>

static inline bool	_prev_is_glued_eq(t_argword *already_parsed, t_token *word)
{
	t_argword	*last;

	last = argword_getlast(already_parsed);
	if (ft_strchr(last->value, '=') && word->glued)
		return (true);
	return (false);
}

t_argword	*export_wildcard_handler(t_argword *args, t_argword *entry, \
																t_token *cur)
{
	static t_token	*prev_word;

	if (args && _prev_is_glued_eq(args, prev_word))
	{
		prev_word = cur;
		return (entry);
	}
	prev_word = cur;
	if (cur && cur->glued && cur->type == TK_ASSIGN)
		return (entry);
	return (replace_wildcards(entry));
}

t_argword	*expand_word(t_sh_ctx *ctx, t_token **cur, bool split,
														bool is_export)
{
	t_argword	*splitted;
	t_argword	*expanded;

	expanded = fill_argword(ctx, cur, is_export);
	if (!expanded)
		return (NULL);
	if (split && expanded->space_offsets.len)
	{
		splitted = split_withespaces(expanded);
		argword_clear(expanded);
		if (!splitted)
			return (NULL);
		expanded = splitted;
	}
	if (is_export)
		return (expanded);
	return (replace_wildcards(expanded));
}

#endif
