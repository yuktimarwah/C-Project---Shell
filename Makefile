CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = PipeDream
SRC = src/main.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
