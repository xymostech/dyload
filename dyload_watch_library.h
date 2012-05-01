/**
 * watch_library.h
 *
 * functions for watching for changes in dynamically
 * loaded libraries, and for reloading them when they
 * change
 */

#ifndef WATCH_LIBRARY_H
#define WATCH_LIBRARY_H

/* currently, this only supports inotify on linux */
#include <linux/inotify.h>
#include <pthread.h>
#include <string.h>

#include <dyload_library.h>

/* start and stop the watcher thread */
void initialize_watcher();
void stop_watcher();

/* tell the watcher to watch a given library */
void watch_library(library_t, dynamic_function_t*);

#endif /* WATCH_LIBRARY_H */
