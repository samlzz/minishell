/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:57:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/06 13:07:02 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

static inline t_token	*_get_tokens(t_hmap *env, const char *input, int16_t *errcode, t_token **errtok)
{
	t_token	*tokens;
	t_token	*expanded;

	tokens = tokenise(input, errcode);
	if (!tokens)
		return (NULL);
	expanded = expand_token_list(env, tokens);
	if (!expanded)
	{
		*errtok = tokens;
		token_pop(&tokens, *errtok);
	}
	token_clear(tokens);
	return (expanded);
}

t_ast	*parse_line(t_hmap *env, const char *input)
{
	t_ast	*res;
	int16_t	errcode;
	t_token	*errtok;
	t_token	*tk_lst;

	errcode = PARSE_OK;
	errtok = NULL;
	tk_lst = _get_tokens(env, input, &errcode, &errtok);
	if (!tk_lst)
		return (print_err(errcode, errtok), NULL);
	res = new_ast(tk_lst, &errtok, &errcode);
	errtok = token_pop(&tk_lst, errtok);
	token_clear(tk_lst);
	if (!res)
		return (print_err(errcode, errtok), NULL);
	if (write_heredocs(env, res))
	{
		token_clear(errtok);
		print_err(PARSE_ERR, NULL);
		ast_free(res);
		return (NULL);
	}
	return (res);
}
