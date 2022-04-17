EXEC=proj2
CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
LDLIBS=
OBJS=proj2.o

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
clean:
	rm $(OBJS) $(EXEC)
