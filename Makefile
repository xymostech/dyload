# compiler
CC=gcc

# general linker flags
LFLAGS=-lpthread -ldl

HEADERS=$(wildcard *.h)

.PHONY:
all: libdyload.so
	cd consoletest; make
	cd gltest; make

# compile the library
libdyload.so: dyload_library.o dyload_watch_library.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^ $(LFLAGS)

# compile the files
%.o: %.c $(HEADERS)
	$(CC) -I. -c -fPIC -o $@ $<

install:
	cp libdyload.so /usr/lib/
	cp dyload.h /usr/include/
	cp dyload_library.h /usr/include/
	cp dyload_watch_library.h /usr/include/

# clean the project
.PHONY: clean
clean:
	rm -f libdyload.so *.o
	cd consoletest; make clean
	cd gltest; make clean
