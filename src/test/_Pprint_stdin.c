#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft.h"

void	get_input(void)
{
	char	*content;
	char	*input;

	content = NULL;
	while (1)
	{
		input = readline("");
		if (!input)
		{
			ft_putendl_fd("exit", 2);
			break;
		}
		content = ft_strappend(content, input);
		if (!content)
			return ;
		free(input);
	}
	printf("########### STDIN CONTENT ##########\n%s", content);
	fflush(stdin);
}

int	main(void)
{
	get_input();
	return (0);
}

