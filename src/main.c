#include <stdio.h>
#include <string.h>
#include "ast.h"


void	print_ast_ascii(t_ast *node);

void	print_err(int16_t err)
{
	if (err == PARSE_ERR_SQUOTE)
		fprintf(stderr, "minishell: unexpected EOF while looking for matching `''\n");
	if (err == PARSE_ERR_DQUOTE)
		fprintf(stderr, "minishell: unexpected EOF while looking for matching `\"'\n");
	else if (err == PARSE_ERR_EOF)
		fprintf(stderr, "minishell: syntax error: unexpected end of file\n");
}


int	main(void)
{
	char	input[1024];
	t_ast	*ast;
	int16_t	err;

	while (1)
	{
		printf("miniparse> ");
		if (!fgets(input, sizeof(input), stdin))
			break;

		// Remove trailing newline
		input[strcspn(input, "\n")] = '\0';

		err = new_ast((const char *)input, &ast);
		if (!ast)
		{
			fprintf(stderr, "Erreur : échec de la création de l'ast. %d\n", err);
			continue;
		}
		print_ast_ascii(ast);
		ast_free(ast);
	}
	printf("Bye!\n");
	return (0);
}

