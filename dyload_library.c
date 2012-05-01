#include "dyload_library.h"

/* mallocs a library and stores the name */
library_t make_library(char* name) {
	library_t library = malloc(sizeof(*library));
	library->library = NULL;
	library->name = name;
	return library;
}

/* loads a library from a file and reads in the functions from the library */
int load_library(library_t library, dynamic_function_t *functions) {
	/* lock the mutex */
	start_use_library();
	/* load the library */
	library->library = dlopen(library->name, RTLD_NOW);
	if(library->library == NULL) {
		/* on error, report, unlock, exit */
		fprintf(stderr, "Error loading library: %s\n", library->name);
		stop_use_library();
		return 1;
	}

	/* enumerate functions */
	while(functions->name) {
		/* load the functions into the given pointers */
		*(void**)functions->var = dlsym(library->library, functions->name);
		if(*(void**)functions->var == NULL) {
			/* on error, report, unlock, exit */
			fprintf(stderr, "Error loading function in library %s: %s\n",
			                library->name, functions->name);
			stop_use_library();
			return 1;
		}
		++functions;
	}

	/* unlock, report success */
	stop_use_library();
	return 0;
}

/* unloads a library and sets the function pointers to null */
int unload_library(library_t library, dynamic_function_t *functions) {
	/* lock the mutex */
	start_use_library();
	if(!library->library) {
		/* if the library isn't loaded, simply return */
		stop_use_library();
		return 0;
	}
	/* unload the library */
	if(dlclose(library->library) != 0) {
		/* on error, report, unlock, exit */
		fprintf(stderr, "Error closing library: %s\n", library->name);
		stop_use_library();
		return 1;
	}

	/* set the library pointer to null */
	library->library = NULL;

	/* enumerate the functions */
	while(functions->name) {
		/* set the pointers to null */
		*(void**)functions->var = NULL;
		++functions;
	}

	/* unlock, report success */
	stop_use_library();
	return 0;
}

/* unloads and reloads a library, and sets the function pointers to the newly loaded library */
int reload_library(library_t library, dynamic_function_t *functions) {
	/* lock the mutex */
	start_use_library();
	/* unload the old library (ensure that it is currently loaded first)*/
	if (library->library && dlclose(library->library) != 0) {
		/* on error, report, unlock, exit */
		fprintf(stderr, "Error closing library %s: %s\n", library->name, dlerror());
		stop_use_library();
		return 1;
	}
	/* load the new library */
	library->library = dlopen(library->name, RTLD_NOW);
	if(library->library == NULL) {
		/* on error, report, unlock, exit */
		fprintf(stderr, "Error loading library: %s\n", library->name);
		stop_use_library();
		return 1;
	}

	/* enumerate functions */
	while(functions->name) {
		/* load the functions into the given pointers */
		*(void**)functions->var = dlsym(library->library, functions->name);
		if(*(void**)functions->var == NULL) {
			/* on error, report, unlock, exit */
			fprintf(stderr, "Error loading function in library %s: %s\n",
			                library->name, functions->name);
			stop_use_library();
			return 1;
		}
		++functions;
	}

	/* unlock, report success */
	stop_use_library();
	return 0;
}

/* the library mutex */
pthread_mutex_t load_mutex = PTHREAD_MUTEX_INITIALIZER;

/* lock the mutex */
void start_use_library() {
	pthread_mutex_lock(&load_mutex);
}

/* unlock the mutex */
void stop_use_library() {
	pthread_mutex_unlock(&load_mutex);
}
