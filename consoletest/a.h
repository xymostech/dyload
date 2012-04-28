#ifndef a_h
#define a_h

#ifdef DYNAMIC
#include "watch_library.h"
#include "library.h"
	/* function to setup the dynamically loading
	 * library */
	void setup_library_a();
	/* reference the pointer to function */
	extern void (*func)(int);
#else
	/* if not dynamic, simply define function like normal */
	void func(int);
#endif

#endif /* A_H */
