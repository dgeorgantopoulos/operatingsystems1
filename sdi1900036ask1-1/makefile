OBJS	= parentchild.o extra.o
SOURCE	= parentchild.c extra.c
HEADER	= extra.h
OUT	= a.out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

server.o: server.c
	$(CC) $(FLAGS) parentchild.c 

extra.o: extra.c
	$(CC) $(FLAGS) extra.c 

clean:
	rm -f $(OBJS) $(OUT)

