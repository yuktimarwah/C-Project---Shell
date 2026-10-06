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

	HANDLE output_handle = NULL;

	HANDLE input_handle = NULL;

if (input_file != NULL)
{
        input_handle = CreateFileA(
                input_file,
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

        if (input_handle == INVALID_HANDLE_VALUE)
        {
                printf("PipeDream: cannot open input file\n");

                if (output_handle != NULL)
                {
                        CloseHandle(output_handle);
                }

                return;
        }

        si.dwFlags |= STARTF_USESTDHANDLES;
        si.hStdInput = input_handle;
}

	if (output_file != NULL)
	{
		output_handle = CreateFileA(
				output_file,
				GENERIC_WRITE,
				0,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

		if (output_handle == INVALID_HANDLE_VALUE)
		{
			printf("PipeDream: cannot open output file\n");
			if (input_handle != NULL)
        {
                CloseHandle(input_handle);
        }
			return;
		}

		si.dwFlags |= STARTF_USESTDHANDLES;
		si.hStdOutput = output_handle;
        }

        if (!CreateProcessA(
                NULL,
                command_line,
                NULL,
                NULL,
                TRUE,
                0,
                NULL,
                NULL,
                &si,
                &pi))
        {
                printf("PipeDream: command not found\n");

                if (output_handle != NULL)
                {
                        CloseHandle(output_handle);
                }

		if (input_handle != NULL)
{
        CloseHandle(input_handle);
}

                return;
        }

        WaitForSingleObject(pi.hProcess, INFINITE);

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        if (output_handle != NULL)
        {
                CloseHandle(output_handle);
        }

#endif
}

void execute_pipe(char *args[], int pipe_index)
{
	#ifdef __linux__

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

	#elif _WIN32

	        SECURITY_ATTRIBUTES sa;

        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.lpSecurityDescriptor = NULL;
        sa.bInheritHandle = TRUE;

        HANDLE read_handle;
        HANDLE write_handle;

        if (!CreatePipe(
                &read_handle,
                &write_handle,
                &sa,
                0))
        {
                printf("PipeDream: cannot create pipe\n");
                return;
        }
		STARTUPINFOA si1;
		STARTUPINFOA si2;
		
		PROCESS_INFORMATION pi1;
		PROCESS_INFORMATION pi2;

        ZeroMemory(&si1, sizeof(si1));
        ZeroMemory(&si2, sizeof(si2));

        ZeroMemory(&pi1, sizeof(pi1));
        ZeroMemory(&pi2, sizeof(pi2));

        si1.cb = sizeof(si1);
        si2.cb = sizeof(si2);

		si1.dwFlags |= STARTF_USESTDHANDLES;
        si1.hStdOutput = write_handle;

        si2.dwFlags |= STARTF_USESTDHANDLES;
        si2.hStdInput = read_handle;

		char command1[500] = "";
		char command2[500] = "";
		
		args[0] = (char *)get_command(args[0]);

for (int i = 0; args[i] != NULL; i++)
{
        strcat(command1, args[i]);
        strcat(command1, " ");
}

args[pipe_index + 1] = (char *)get_command(args[pipe_index + 1]);

for (int i = pipe_index + 1; args[i] != NULL; i++)
{
        strcat(command2, args[i]);
        strcat(command2, " ");
}

char command_line1[600];

snprintf(command_line1, sizeof(command_line1),
         "cmd.exe /C \"%s\"", command1);

if (!CreateProcessA(
        NULL,
        command_line1,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si1,
        &pi1))
{
        printf("PipeDream: command not found\n");
        CloseHandle(read_handle);
        CloseHandle(write_handle);
        return;
}

char command_line2[600];

snprintf(command_line2, sizeof(command_line2),
         "cmd.exe /C \"%s\"", command2);

if (!CreateProcessA(
        NULL,
        command_line2,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &si2,
        &pi2))
{
        printf("PipeDream: command not found\n");

        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        CloseHandle(read_handle);
        CloseHandle(write_handle);

        return;
}

WaitForSingleObject(pi1.hProcess, INFINITE);
WaitForSingleObject(pi2.hProcess, INFINITE);

CloseHandle(pi1.hProcess);
CloseHandle(pi1.hThread);

CloseHandle(pi2.hProcess);
CloseHandle(pi2.hThread);

CloseHandle(read_handle);
CloseHandle(write_handle);


	#endif
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
