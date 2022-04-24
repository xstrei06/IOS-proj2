EXEC=proj2
CC=gcc
CFLAGS=-std=gnu99 -pthread -Wall -Wextra -Werror -pedantic
LDLIBS=-lpthread
OBJS=proj2.o

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDLIBS)
%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@
clean:
	rm $(OBJS) $(EXEC)
