CC= gcc
/cflags = -Wall -O0
MAIN = lab7
OBJS = matmul2.o
all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

matmul2.o : matmul2.c
	$(CC) $(CFLAGS) -c matmul2.c

clean :
	rm *.o $(MAIN) core

