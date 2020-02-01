CC=gcc                # Select Compiler (gcc)
CFLAGS=-o             # 
FILES=lab3.c          # File names to be compiled

Lab3Test: $(FILES)       
	$(CC) $(FILES) $(CFLAGS) lab3

# force erases all .o files and the executable for a new clean compilation
clean:
	rm -f lab3  

all: Lab3Test
# Run the make file as 'make all'