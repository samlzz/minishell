#include <stdio.h>

int main(int argc, char const *argv[])
{
	printf("ARGV: [");
	for (size_t i = 0; i < (size_t)argc; i++)
	{
		if (i + 1 < (size_t)argc)
			printf("%s, ", argv[i]);
		else
			printf("%s]\n", argv[i]);
	}
	return 0;
}
