CC=g++
CFLAGS=-I
CFLAGS+=-Wall
FILES1=Lab5.cpp
FILES2=procMonitor.cpp

sysmonExec: $(FILES1)
	$(CC) $(CFLAGS) -o lab5 $(FILES1)

intfMonitor: $(FILES2)
	$(CC) $(CFLAGS) -o procMonitor $(FILES2)

clean:
	rm -f *.o lab5 procMonitor

all: sysmonExec intfMonitor