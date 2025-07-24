/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 14:17:47 by sliziard          #+#    #+#             */
/*   Updated: 2025/07/24 10:48:31 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include "libft.h"
#include "token.h"
#include "utils.h"
#include "error/error.h"

static inline int32_t	_handle_quoted_word(const char *input, t_token *curr)
{
	int32_t	len;

	len = 1;
	while (input[len] && input[len] != *input)
		len++;
	if (!input[len])
	{
		if (*input == '"')
			return (PARSE_ERR_DQUOTE);
		return (PARSE_ERR_SQUOTE);
	}
	curr->value = ft_substr(input, 1, len - 1);
	if (!curr->value)
		return (perror("minishell: tokenise: malloc"), PARSE_ERR);
	if (*input == '"')
		curr->quote = QUOTE_DOUBLE;
	else
		curr->quote = QUOTE_SINGLE;
	return (len + 1);
}

static inline int32_t	_handle_word(const char *input, t_token *curr)
{
	int32_t	len;

	if (*input == '=')
	{
		curr->type = TK_ASSIGN;
		curr->value = ft_strdup("=");
		if (!curr->value)
			return (PARSE_ERR);
		return (1);
	}
	curr->type = TK_WORD;
	if (*input == '"' || *input == '\'')
		return (_handle_quoted_word(input, curr));
	len = 0;
	while (input[len] && !ft_isspace(input[len]) && \
		!ft_strchr(HANDLED_CHAR, input[len]))
		len++;
	if (!len)
		return (PARSE_ERR_MALFORMED);
	curr->value = ft_substr(input, 0, len);
	if (!curr->value)
		return (perror("minishell: tokenise: malloc"), PARSE_ERR);
	curr->quote = QUOTE_NONE;
	return (len);
}

static int32_t	_fill_token(const char *input, t_token *curr)
{
	if (!*input)
		return (curr->type = TK_EOF, 0);
	else if (*input == '(')
		curr->type = TK_LPAREN;
	else if (*input == ')')
		curr->type = TK_RPAREN;
	else if (*input == '|' && input[1] == '|')
		curr->type = TK_OR;
	else if (*input == '|')
		curr->type = TK_PIPE;
	else if (*input == '&' && input[1] == '&')
		curr->type = TK_AND;
	else if (*input == '&')
		return (PARSE_ERR_SOLO_AND);
	else if (input[0] == '>')
		curr->type = (input[1] == '>') * TK_REDIR_APPEND
			+ !(input[1] == '>') * TK_REDIR_OUT;
	else if (input[0] == '<')
		curr->type = (input[1] == '<') * TK_HEREDOC
			+ !(input[1] == '<') * TK_REDIR_IN;
	else
		return (_handle_word(input, curr));
	return (1 + (curr->type == TK_OR || curr->type == TK_AND \
		|| curr->type == TK_REDIR_APPEND || curr->type == TK_HEREDOC));
}

static inline t_token	*_create_token(const char *input, size_t *pos,
																bool is_first)
{
	t_token	*tk;
	size_t	save;

	tk = ft_calloc(1, sizeof (t_token));
	if (!tk)
		return (perror("minishell: tokenise: malloc"), NULL);
	tk->glued = !is_first;
	save = *pos;
	while (input[*pos] && ft_isspace(input[*pos]))
		(*pos)++;
	if (*pos != save)
		tk->glued = false;
	return (tk);
}

/**
 * @brief Tokenizes an input string into a linked list of tokens.
 *
 * This function splits the input command line into meaningful tokens (words,
 * pipes, redirections, etc.) for further parsing into an AST.
 *
 * @param input       The raw input string from the user.
 * @param exit_code   A pointer to an int16_t to store the error code:
 *                    -  0 : success
 *                    - -1 : malloc failure
 *                    - -2 : unclosed single quote
 *                    - -3 : unclosed double quote
 *                    - -4 : some symbol is malformed
 *                    - -6 : a lonely '&' was encountered
 *
 * @return A linked list of tokens on success, or NULL on error.
 */

t_token	*tokenise(const char *input, int16_t *exit_code)
{
	size_t	i;
	int32_t	offset;
	t_token	*tokens;
	t_token	*curr;

	i = 0;
	offset = 0;
	tokens = NULL;
	while (input[i])
	{
		i += offset;
		curr = _create_token(input, &i, tokens == NULL);
		if (!curr)
			return (token_clear(tokens), *exit_code = PARSE_ERR, NULL);
		offset = _fill_token(input + i, curr);
		if (offset < 0)
		{
			*exit_code = (int16_t)offset;
			token_clear(tokens);
			free(curr);
			return (NULL);
		}
		token_addback(&tokens, curr);
	}
	return (tokens);
}
