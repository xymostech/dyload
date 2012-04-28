# compiler
CC=gcc

# general compiler flags
CFLAGS=-I../
# general linker flags
LFLAGS=

# non-dynamically loading files
OBJ=consoletest
# dynamically loading files
DYOBJ=a
# list of headers
HEADERS=$(wildcard *.h)

# final product
PRODUCT=consoletest

# compile both static and dynamic versions, and all
# dynamically loading files
.PHONY: all
all: dynamic static $(DYOBJ)


# =====  dynamic version =======
.PHONY: dynamic
dynamic: $(PRODUCT)-dynamic

# assemble the final dynamic version
$(PRODUCT)-dynamic: $(addsuffix .noso-d.o, $(OBJ)) $(addsuffix _lib.noso-d.o, $(DYOBJ))
	$(CC) -L../ -o $@ $^ $(LFLAGS) -ldyload

# compile non-dynamic with-dynamic-code object files
%.noso-d.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -DDYNAMIC -c -o $@ $<


# compile each dynamic object file
.PHONY: $(DYOBJ)
.SECONDEXPANSION:
$(DYOBJ): $$@.so

# for safety, compile to a separate file and move to the final location
%.so: %-new.so
	mv $< $@

# link the new library from its object file
%-new.so: %.so.o
	$(CC) -shared -Wl,-soname,$@ -o $@ $^ $(LFLAGS)

# compile the dynamic object files
%.so.o: %.c
	$(CC) $(CFLAGS) -c -fPIC -o $@ $<


# ======  static version ======
.PHONY: static
static: $(PRODUCT)

# assemble the final static version
$(PRODUCT): $(addsuffix .noso-s.o, $(OBJ)) $(addsuffix .noso-s.o, $(DYOBJ))
	$(CC) -o $@ $^ $(LFLAGS)

# compile non-dynamic no-dynamic-code object files
%.noso-s.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<


# clean the project
.PHONY: clean
clean:
	rm -f $(PRODUCT) $(PRODUCT)-dynamic *.o *.so
