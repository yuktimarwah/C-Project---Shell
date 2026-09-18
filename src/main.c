#include <stdio.h>

int main (void) {

	char command[100];

	while (1) {

		printf("PipeDream$ ");

		fgets(command, 100, stdin);

		printf("You entered: %s", command);
	}

	return 0;
}
