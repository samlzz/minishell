/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 19:21:54 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/29 08:41:05 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "expander.h"
# include "lexer/token.h"
# include "libft.h"
# include <stdbool.h>

#ifdef MINISHELL_BONUS

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

t_argword	*expand_word_handler(t_sh_ctx *ctx, t_token **cur, bool split,
																bool is_export)
{
	t_argword	*splitted;
	t_argword	*expanded;

	expanded = fill_argword(ctx, cur, is_export);
	if (!expanded)
		return (NULL);
	if (split && expanded->space_offsets.len)
	{
		splitted = field_splitting(expanded);
		argword_clear(expanded);
		if (!splitted)
			return (NULL);
		expanded = splitted;
	}
	if (is_export)
		return (expanded);
	return (replace_wildcards(expanded));
}

#else

t_argword	*expand_word_handler(t_sh_ctx *ctx, t_token **cur, bool split,
																bool is_export)
{
	t_argword	*splitted;
	t_argword	*expanded;

	expanded = fill_argword(ctx, cur, is_export);
	if (!expanded)
		return (NULL);
	if (split && expanded->space_offsets.len)
	{
		splitted = field_splitting(expanded);
		argword_clear(expanded);
		if (!splitted)
			return (NULL);
		expanded = splitted;
	}
	return (expanded);
}


#endif
