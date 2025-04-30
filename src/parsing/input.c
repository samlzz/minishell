/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:57:40 by sliziard          #+#    #+#             */
/*   Updated: 2025/04/30 17:59:50 by sliziard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

//TODO: tmp
void	print_ast_ascii(t_ast *node);
#define EXEC(node)	print_ast_ascii(node)

static inline bool	_skipable(const char *input)
{
	size_t	i;

	if (!ft_strncmp(input, "\n", 1))
		return (true);
	if (!ft_strncmp(input, ":", 1))
		return (true);
	if (!ft_strncmp(input, "!", 1))
		return (true);
	i = 0;
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (!input[i]);
}

void	get_input(t_hmap *env)
{
	char	*input;
	int16_t	err;
	t_token	*errtok;
	t_ast	*ast;

	while (1)
	{
		input = readline(CMD_PROMPT);
		if (!input)
		{
			ft_putendl_fd("exit", 2);
			break;
		}
		if (*input)
			add_history(input);
		if (!_skipable(input))
		{
			ast = parse_input(env, input, &err, &errtok);
			if (!ast)
				print_err(err, errtok);
			else
				EXEC(ast);			
			ast_free(ast);
		}
		free(input);
	}
}

/**
 * @brief Full parsing routine from input string to AST.
 * 
 * Tokenizes the input, expands the tokens using env, parses into an AST,
 * and handles heredoc collection. Cleans up intermediate structures on failure.
 * 
 * @param env The environment hashmap used for expansion.
 * @param input The raw user input string.
 * @param err_code Output error code.
 * @param errtok Token that caused a parse error (if applicable).
 * @return t_ast* The parsed AST or NULL on failure.
 * @see print_err
 */
t_ast	*parse_input(t_hmap *env, const char *input, int16_t *err_code, \
	t_token **errtok)
{
	t_token	*tokens;
	t_token	*expanded;
	t_ast	*ast;

	*err_code = PARSE_OK;
	*errtok = NULL;
	tokens = tokenise(input, err_code);
	if (!tokens)
		return (NULL);
	expanded = expand_token_list(env, tokens);
	if (!expanded)
		(*errtok = tokens, token_pop(&tokens, *errtok));
	token_clear(tokens);
	if (!expanded)
		return (NULL);
	ast = new_ast(expanded, errtok, err_code);
	*errtok = token_pop(&expanded, *errtok);
	token_clear(expanded);
	if (write_heredocs(env, ast))
	{
		token_clear(*errtok);
		*errtok = NULL;
		return (ast_free(ast), NULL);
	}
	return (ast);
}
