CC=g++                # Select Compiler (g++)
CFLAGS=-I             # Add search for .h header files
CFLAGS+=-Wall         # Includes extra warnings during the compilation process 
FILES=lab2.cpp        # File names to be compiled
LIBS = -L. -lPidUtil  # -L adds search for libraries, -L. lib will be in the same folder, then add lib name

# build command as 'g++ -I -Wall lab2.cpp -L. -lPidUtil'
# -o indicates that the output is Lab2 (-o Lab2)
Lab2Test: $(FILES)       
	$(CC) $(CFLAGS) $(FILES) -o Lab2 $(LIBS) 

# force erases all .o files and the executable for a new clean compilation
clean:
	rm -f *.o lab2  

all: Lab2Test