CC=g++
CFLAGS=-I
CFLAGS+=-Wall
FILES=Server.cpp
LIBS=-lpthread

server: $(FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f *.o Server

all: server
