CC=g++
CFLAGS=-I
CFLAGS+=-Wall
FILES1=networkMonitor.cpp
FILES2=ifMonitor.cpp

netMon: $(FILES1)
	$(CC) $(CFLAGS) -o networkMonitor $(FILES1)

ifMon: $(FILES2)
	$(CC) $(CFLAGS) -o ifMonitor $(FILES2)

clean:
	rm -f *.o networkMonitor ifMonitor

all: netMon ifMon