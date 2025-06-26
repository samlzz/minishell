/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argword_fill.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 13:00:29 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/26 12:30:57 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "env/env.h"
#include <stdlib.h>

/**
 * @brief Appends the value of a variable to the buffer from the input string.
 * 
 * Extracts a valid variable name (alphanumeric or '_'), looks it up in the
 * environment, and appends its value to the buffer.
 * 
 * @param env Environment hashmap.
 * @param buf Buffer to write into.
 * @param input Pointer to the variable name start (after `$`).
 * @return int32_t Number of characters read from input, or -1 on error.
 */
static int32_t	_put_value(t_sh_ctx *ctx, t_dynbuf *buf, \
	const char *input)
{
	int32_t	i;
	char	*key;
	char	*val;

	i = 0;
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	key = ft_substr(input, 0, i);
	if (!key)
		return (-1);
	val = ft_hmap_get(&ctx->env, key);
	if (!ft_strcmp(key, "PATH") && !val && ctx->use_fallback_path)
		val = ENV_DEFAULT_PATH;
	free(key);
	if (val && !ft_dynbuf_append_str(buf, val))
		return (-1);
	return (i);
}

/**
 * @brief Fills the dynamic buffer based on the current input character.
 * 
 * Handles shell expansion logic: `$VAR`, `$?`, `$0`, or normal character.
 * 
 * @param env Environment hashmap.
 * @param input Pointer to the current input string position.
 * @param buf Buffer to write into.
 * @param last_exit String version of last exit code (used for `$?`).
 * @return true on error, false on success.
 */
static inline bool	_fill_buffer(t_sh_ctx *ctx, const char **input, \
	t_dynbuf *buf, char *last_exit)
{
	int32_t	read;

	if (**input == '$' && (*input)++)
	{
		if (**input == '$' && (*input)++)
			return (!ft_dynbuf_append_str(buf, ctx->p_id));
		else if (**input == '?' && (*input)++)
			return (!ft_dynbuf_append_str(buf, last_exit));
		else if (**input >= '0' && **input <= '9')
		{
			if (**input == '0' && !ft_dynbuf_append_str(buf, ctx->p_name))
				return (true);
			return ((*input)++, false);
		}
		else if (ft_isalpha(**input) || **input == '_')
		{
			read = _put_value(ctx, buf, *input);
			*input += read;
			return (read == -1);
		}
		else
			return (!ft_dynbuf_append_char(buf, '$'));
	}
	else
		return (!ft_dynbuf_append_char(buf, *(*input)++));
}

/**
 * @brief Expands the value of a token, performing variable substitutions.
 * 
 * Handles `$`, `$VAR`, `$?`, `$0`, with quote context. If in single quotes,
 * returns the value unmodified.
 * 
 * @param env Environment hashmap.
 * @param cur Current token to expand.
 * @return char* Expanded value string (must be freed), or NULL on error.
 */
static char *_expand_token(t_sh_ctx *ctx, t_token *cur)
{
	t_dynbuf	buf;
	char		*exit_code;
	const char	*input;

	if (cur->quote == QUOTE_SINGLE)
		return (ft_strdup(cur->value));
	if (!ft_strcmp(cur->value, "$"))
		return (ft_strdup("$"));
	input = cur->value;
	buf = ft_dynbuf_new(ft_strlen(cur->value));
	if (!buf.data)
		return (NULL);
	exit_code = ft_itoa(ctx->lst_exit);
	if (!exit_code)
		return (ft_dynbuf_free(&buf), NULL);
	while (*input)
	{
		if (_fill_buffer(ctx, &input, &buf, exit_code))
			return (free(exit_code), ft_dynbuf_free(&buf), NULL);
	}
	return (free(exit_code), buf.data);
}

/**
 * @brief Build an argword structure from a sequence of glued TK_WORD tokens.
 * 
 * Expands values (unless in single quotes), tracks quote state, and merges
 * tokens until unglued token is reached.
 * 
 * @param env Environment hashmap.
 * @param cur Pointer to the current token pointer (will be moved).
 * @return t_argword* Newly allocated argword node, or NULL on error.
 */
t_argword	*fill_argword(t_sh_ctx *ctx, t_token **cur)
{
	t_argword	*node;
	char		*expanded;
	int16_t		err;

	node = argword_new();
	if (!node)
		return (NULL);
	while (*cur && (*cur)->type == TK_WORD)
	{
		expanded = _expand_token(ctx, *cur);
		if (!expanded)
			return (argword_clear(node), NULL);
		err = !argword_append_value(node, expanded, (*cur)->quote);
		free(expanded);
		if (err)
			return (argword_clear(node), NULL);
		next(cur);
		if (*cur && !(*cur)->glued)
			break ;
	}
	return (node);
}
