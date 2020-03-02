CC=g++
CFLAGS=-I
CFLAGS+=-Wall
FILES1=netMonitor.cpp
FILES2=ifMonitor.cpp

netMon: $(FILES1)
	$(CC) $(CFLAGS) -o netMonitor $(FILES1)

ifMon: $(FILES2)
	$(CC) $(CFLAGS) -o ifMonitor $(FILES2)

clean:
	rm -f *.o netMonitor procMonitor

all: netMon ifMon