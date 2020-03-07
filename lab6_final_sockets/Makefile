CC=g++
CFLAGS=-I
CFLAGS+=-Wall
FILES=lab6.cpp
FILES1+=client.cpp

lab6: $(FILES)
	$(CC) $(CFLAGS) $^ -o $@

client: $(FILES1)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o lab6 client

all: lab6 client


