CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L -Iinclude
SRCS = src/main.c src/parser.c src/commands.c src/storage.c src/state.c src/utils.c
OBJS = $(SRCS:.c=.o)
TARGET = todo

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test_utils:
	$(CC) $(CFLAGS) src/utils.c tests/test_utils.c -o tests/test_utils
	./tests/test_utils