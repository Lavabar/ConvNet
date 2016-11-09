CC=clang
NETSRCS=conv.c iplimage.c #relu.c pool.c
CFLAGS=-Wall -g -lpng12 #-lm #`pkg-config --cflags` 
NETOBJS=$(NETSRCS:.c=.o)

all: main

main: $(NETOBJS) main.o
	$(CC) $(CFLAGS) -o main main.o $(NETOBJS)

main.o:main.c
	$(CC) $(CFLAGS) -c main.c

conv.o:conv.c
	$(CC) $(CFLAGS) -c conv.c

#relu.o:relu.c
#	$(CC) $(CFLAGS) -c relu.c

#pool.o:pool.c
#	$(CC) $(CFLAGS) -c pool.c

clean:
	rm main *.o 
