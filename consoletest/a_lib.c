#include "a.h"

/* the pointer to the function */
void (*func)(int);

/* initialize the library with one function */
INIT_LIBRARY(a, 1, FUNC(func));

void setup_library_a() {
	/* setup the library */
	SETUP_LIBRARY(a, "./a.so");
}
