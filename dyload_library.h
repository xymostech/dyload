/**
 * library.h
 *
 * Functions for handling dynamically loading libraries and
 * for setting given pointers to the loaded functions, for use
 * in the rest of the program
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <stdlib.h>

#define __USE_GNU
#include <dlfcn.h>
#include <pthread.h>

/**
 * dynamic_function_t
 *
 * Holds information about a single dynamic function
 *
 * name: string version of the name
 *  var: pointer to the global definition of the function
 */
typedef struct {
	char *name;
	void *var;
} dynamic_function_t;

/**
 * library_t
 *
 * Holds information about a dynamic library
 *
 *    name: string version of the file name
 * library: pointer to the library in memory
 */
typedef struct {
	char *name;
	void *library;
} *library_t;

/* initialize a library, given its file name */
library_t make_library(char*);

/**
 * load_library, unload_library, reload_library
 *
 * loads, unloads, and reloads a library to and from memory
 * using the stored file name of the library
 *
 * loads all of the functions stored in the array passed
 * as the second argument
 */
int load_library(library_t, dynamic_function_t*);
int unload_library(library_t, dynamic_function_t*);
int reload_library(library_t, dynamic_function_t*);

/**
 * start_use_library, stop_use_library
 *
 * Locks and unlocks the library mutex to ensure that
 * library functions are not accessed while they are being
 * reloaded
 *
 * These must encapsulate every usage of the dynamically
 * loaded functions
 */
void start_use_library();
void stop_use_library();

/**
 * INIT_LIBRARY, FUNC, SETUP_LIBRARY
 *
 * macros to help design a dynamic library and its 
 * cooresponding functions
 */
#define INIT_LIBRARY(name, nfuncs, funcs...) library_t library_##name; \
dynamic_function_t funcs_##name[(nfuncs)+1] = {funcs, NULL}

#define FUNC(func) {#func, &func}

#define SETUP_LIBRARY(name, file) library_##name = make_library(file); \
load_library(library_##name, funcs_##name); \
watch_library(library_##name, funcs_##name)

/**
 * Example usage:
 *
 * INIT_LIBRARY(my_library, 2, FUNC(my_func1), FUNC(my_func2));
 *
 * setup_library_my_library() {
 *     SETUP_LIB(my_library, "./my_library.so");
 */

#endif /* LIBRARY_H */
