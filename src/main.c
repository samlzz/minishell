#include <stdio.h>
#include <string.h>
#include "ast.h"
#include <stdlib.h>


void	print_ast_ascii(t_ast *node);

static inline char	*token_type_str(t_tk_type type)
{
	if (type == TK_WORD) return "WORD";
	if (type == TK_PIPE) return "|";
	if (type == TK_AND) return "&&";
	if (type == TK_OR) return "||";
	if (type == TK_REDIR_IN) return "<";
	if (type == TK_REDIR_OUT) return ">";
	if (type == TK_REDIR_APPEND) return ">>";
	if (type == TK_HEREDOC) return "<<";
	if (type == TK_EOF) return "EOF";
	return "UNKNOWN";
}
void	print_err(int16_t errcode, t_token *errtok)
{
	char	*tk;

	if (errcode == PARSE_ERR)
		return (ft_putendl_fd("minishell: internal error occurs", 2));
	else if (errcode == PARSE_ERR_SQUOTE)
		return (ft_putendl_fd("minishell: unexpected EOF while looking for matching `\''", 2));
	else if (errcode == PARSE_ERR_DQUOTE)
		return (ft_putendl_fd("minishell: unexpected EOF while looking for matching `\"'", 2));
	if (!errtok && errcode == PARSE_OK)
		tk = "newline";
	else
		tk = errtok->value;
	if (!tk)
		tk = token_type_str(errtok->type);
	fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n", tk);
	token_clear(errtok);
}


int	main(void)
{
	char	input[1024];
	t_ast	*ast;
	int16_t	err;
	t_token	*errtok;

	while (1)
	{
		ast = NULL;
		printf("miniparse> ");
		if (!fgets(input, sizeof(input), stdin))
			break;
		// Remove trailing newline
		input[strcspn(input, "\n")] = '\0';

		ast = new_ast((const char *)input, &err, &errtok);
		if (!ast)
		{
			ft_putendl_fd("", 2);
			print_err(err, errtok);
			continue;
		}
		print_ast_ascii(ast);
		ast_free(ast);
	}
	printf("Bye!\n");
	return (0);
}

