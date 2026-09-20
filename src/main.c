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

		if (strcmp(args[0], "cd") == 0)
		{
			if (args[1] == NULL)
			{
				printf("PipeDream: expected directory\n");
			}
			else if (chdir(args[1]) != 0)
			{
				printf("PipeDream: no such directory\n");
			}
			
			continue;

			}

		if (strcmp(args[0], "pwd") == 0)
		{
			char cwd[1024];

			getcwd(cwd, sizeof(cwd));
			printf("%s\n", cwd);

			continue;
		}

		if (strcmp(args[0], "echo") == 0) 
		{
			int i = 1;
			
			while (args[i] != NULL) 
			{
				printf("%s ", args[i]);
				i++;
			}
			
			printf("\n");

			continue;

			}

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
