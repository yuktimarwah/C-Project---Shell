#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main (void)
{
	char input[100];
	char *args[10];

	while (1) 
	{

		printf("PipeDream$ ");

		fgets(input, 100, stdin);
		input[strcspn(input, "\n")] = '\0';

		int count = 0;

		char *token = strtok(input, " ");

		while (token != NULL && count < 9)
		{

			args[count] = token;
			count++;

			token = strtok(NULL, " ");
		}

		args[count] = NULL;

		pid_t pid = fork();

		if (pid == 0)
		{
			execvp(args[0], args);
			printf("PipeDream: command not found\n");
		}
		else
		{
			wait(NULL);
		}
	}

	return 0;
}
