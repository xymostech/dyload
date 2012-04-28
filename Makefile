# compiler
CC=gcc

# general linker flags
LFLAGS=-lpthread -ldl

OBJ=library watch_library
HEADERS=$(wildcard *.h)

# compile the library
libdyload.so: library.o watch_library.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^ $(LFLAGS)

# compile the files
%.o: %.c $(HEADERS)
	$(CC) -c -fPIC -o $@ $<

# clean the project
.PHONY: clean
clean:
	rm -f libdyload.so *.o
