#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#endif

const char *get_os()
{
#ifdef _WIN32
	return "Windows";
#elif __linux__
	return "Linux";
#else
	return "Unknown";
#endif
}


const char *get_command(const char *command)
{
#ifdef _WIN32
	if (strcmp(command, "list") == 0)
	{
		return "dir";
	}
	if (strcmp(command, "show") == 0)
	{
		return "type";
	}
	if (strcmp(command, "clear") == 0) 
	{
		return "cls";
	}

#elif __linux__
	if (strcmp(command, "list") == 0)
	{
		return "ls";
	}
	if (strcmp(command, "show") == 0)
	{
		return "cat";
	}
	if (strcmp(command, "clear") == 0)
	{
		return "clear";
	}
#endif
	return command;
}

void execute_command(char *args[])
{
        args[0] = (char *)get_command(args[0]);

#ifdef __linux__

        execvp(args[0], args);

        printf("PipeDream: command not found\n");

#endif

}

void execute_process(char *args[], char *output_file, char *input_file)
{
#ifdef __linux__

        pid_t pid = fork();

        if (pid == 0)
       {
                if (output_file != NULL)
                {
                        int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                        if (fd == -1)
                        {
                                printf("PipeDream: cannot open output file\n");
                                return;
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
                                return;
                        }

                        dup2(fd, STDIN_FILENO);
                        close(fd);
                }

                execute_command(args);
        }
        else
        {
                wait(NULL);
        }

#elif _WIN32

	args[0] = (char *)get_command(args[0]);

	char command[1000] = "";

        for (int i = 0; args[i] != NULL; i++)
        {
                strcat(command, args[i]);
                strcat(command, " ");
        }

        char command_line[1100];

        snprintf(command_line, sizeof(command_line),
                 "cmd.exe /C \"%s\"", command);

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        ZeroMemory(&pi, sizeof(pi));

        si.cb = sizeof(si);

        if (!CreateProcessA(
                NULL,
                command_line,
                NULL,
                NULL,
                FALSE,
                0,
                NULL,
                NULL,
                &si,
                &pi))
        {
                printf("PipeDream: command not found\n");
                return;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

#endif
}

void execute_pipe(char *args[], int pipe_index)
{
	int fd[2];
	pipe(fd);

	pid_t pid1 = fork();

	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);

		close(fd[0]);
		close(fd[1]);

		args[0] = (char *)get_command(args[0]);

		execvp(args[0], args);

		printf("PipeDream: command not found\n");
		return;
	}

	pid_t pid2 = fork();

	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);

		close(fd[0]);
		close(fd[1]);

		execvp(args[pipe_index + 1], &args[pipe_index + 1]);

		printf("PipeDream: command not found\n");
		return;
	}

	close(fd[0]);
	close(fd[1]);

	wait(NULL);
	wait(NULL);
}


int main (void)
{
	printf("Running on: %s\n", get_os());

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
			else if (
#ifdef _WIN32
					_chdir(args[1])
#else
					chdir(args[1])
#endif
					!= 0)
			{
				printf("PipeDream: no such directory\n");
			}
			
			continue;

			}

		if (strcmp(args[0], "pwd") == 0)
		{
			char cwd[1024];

			#ifdef _WIN32
        _getcwd(cwd, sizeof(cwd));
#else
        getcwd(cwd, sizeof(cwd));
#endif
			printf("%s\n", cwd);

			continue;
		}


		if (strcmp(args[0], "exit") == 0) {
			
			return 0;

		}

		char *output_file = NULL;

		char *input_file = NULL;

		int pipe_index = -1;

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

			else if (strcmp(args[i], "|") == 0)
			{
				pipe_index = i;
				args[i] = NULL;
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

		if (pipe_index != -1)
		{
			execute_pipe(args, pipe_index);
			continue;
		}

		execute_process(args, output_file, input_file);


		
	}

	return 0;
}
