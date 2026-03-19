CC = gcc
CFLAGS = -Wall -Wextra
OBJS = main.o input.o parser.o executor.o
EXEC = seashell

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)