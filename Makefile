# compiler
CC=gcc

# general linker flags
LFLAGS=-lpthread -ldl

OBJ=library watch_library
HEADERS=$(wildcard *.h)

.PHONY:
all: libdyload.so
	cd consoletest; make

# compile the library
libdyload.so: library.o watch_library.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^ $(LFLAGS)

# compile the files
%.o: %.c $(HEADERS)
	$(CC) -c -fPIC -o $@ $<

install:
	cp libdyload.so /usr/lib/

# clean the project
.PHONY: clean
clean:
	rm -f libdyload.so *.o
	cd consoletest; make clean
