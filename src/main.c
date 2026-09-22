#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

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


		if (strcmp(args[0], "exit") == 0) {
			
			return 0;

		}

		char *output_file = NULL;

		char *input_file = NULL;

		int i = 0;

		while (args[i] != NULL) 
		{
			if (strcmp(args[i], ">") == 0)
			{
				if (args[i + 1] != NULL) 
				{
					output_file = args[i + 1];
					args[i] = NULL;
				}
			}

			else if (strcmp(args[i], "<") == 0)
			{
				if (args[i + 1] != NULL)
				{
					input_file = args[i + 1];
					args[i] = NULL;
				}
			}

			i++;
		}

		if (strcmp(args[0], "echo") == 0 && output_file == NULL)
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
			if (output_file != NULL)
			{
				int fd = open(output_file,O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd == -1) 
				{
					printf("PipeDream: cannot open output file\n");
					return 1;
				}

				dup2(fd, STDOUT_FILENO);

				close(fd);
			}

			if (input_file != NULL) 
			{
				int fd = open(input_file, O_RDONLY);

				if (fd == -1) 
				{
					printf("PipeDream: cannot open input file\n");
					return 1;
				}

				dup2(fd, STDIN_FILENO);

				close(fd);
			}

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
