CC = gcc
CFLAGS = -Wall -Wextra

SRC_DIR = src
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/input.c $(SRC_DIR)/parser.c $(SRC_DIR)/executor.c
OBJS = $(SRCS:.c=.o)
EXEC = seashell

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)