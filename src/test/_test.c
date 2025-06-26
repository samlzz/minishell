#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>

void sig_handle(int sig)
{
	rl_replace_line("", 0);
	rl_done = 1;
}

int event(void)
{
	return (0);
}

void	minishell_suite(void)
{
	printf("lalalala\n");
}

int main(int argc, char const *argv[])
{
	char	*input;

	rl_event_hook=&event;
	signal(SIGINT, &sig_handle);
	while (1)
	{
		input = readline(">>");
		// ctrl + c: input == ""
		printf("INPUT [%s]\n", input);
		if (!input || !*input)
		{
			if (!*input)
				free(input);
			fprintf(stderr, "exit\n");
			break;
		}
		if (*input)
			add_history(input);
		free(input);
	}
	minishell_suite();
	return 0;
}
