#include <stdio.h>
#include <stdlib.h>

#include <dyload.h>

/* include the header like normal */
#include "a.h"

int main(int argc, char **argv) {
#ifdef DYNAMIC
	/* start up the watch thread and
	 * load the library */
	initialize_watcher();
	setup_library_a();
#endif

	/* a count variable */
	int count = 0;

	while(1) {
#ifdef DYNAMIC
		/* need to ensure synchronization of library
		 * loading and calling functions, so we need to
		 * wrap the function call in locks */
		start_use_library();
#endif

		/* call the function */
		func(count);

#ifdef DYNAMIC
		stop_use_library();
#endif
		sleep(2);
		count++;
	}

	return 0;
}
