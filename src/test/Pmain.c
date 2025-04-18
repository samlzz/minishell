#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"

void		print_ast_ascii(t_ast *node);
const char	*token_type_str(t_tk_type type);

void	print_err(int16_t errcode, t_token *errtok)
{
	const char	*tk;

	if (errcode == PARSE_ERR)
		return (ft_putendl_fd("minishell: internal error occurs", 2));
	else if (errcode == PARSE_ERR_SQUOTE)
		return (ft_putendl_fd("minishell: unexpected EOF while looking for matching `\''", 2));
	else if (errcode == PARSE_ERR_DQUOTE)
		return (ft_putendl_fd("minishell: unexpected EOF while looking for matching `\"'", 2));
	if (!errtok && errcode == PARSE_OK)
		tk = "newline";
	else
		tk = (const char *)errtok->value;
	if (!tk)
		tk = token_type_str(errtok->type);
	fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tk);
	fprintf(stderr, "[%d]\n", errcode);
	token_clear(errtok);
}

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void	get_input(void)
{
	char	*input;
	int16_t	err;
	t_token	*errtok;
	t_ast	*ast;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			ft_putendl_fd("exit", 2);
			break;
		}
		if (*input)
			add_history(input);
		ast = new_ast(input, &err, &errtok);
		if (!ast)
			print_err(err, errtok);
		else //todo: exec
			print_ast_ascii(ast);
		free(input);
		ast_free(ast);
	}
}

int	main(void)
{
	get_input();
	return (0);
}

