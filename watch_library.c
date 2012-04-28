#include "watch_library.h"

/* some useful definitions */
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUFFER_SIZE (32*(EVENT_SIZE+16))

/* return the directory portion of a file */
char *get_directory(char *file) {
	int i, hold = 0;
	for(i=0; *(file+i); ++i) {
		if(*(file+i) == '/') {
			hold = i;
		}
	}
	char *dir = malloc(hold+2);
	strncpy(dir, file, hold+1);
	dir[hold+1] = '\0';
	return dir;
}

/* return the file portion of a file */
char *get_file(char *file) {
	char *hold = file;
	while(*file) {
		if(*file == '/') {
			hold = file+1;
		}
		++file;
	}
	return hold;
}

/**
 * watch_node_t
 *
 * holds information about a given inotify
 * watch on a library
 *
 * stored in linked list fashion
 */
typedef struct watch_node {
	int watch;
	library_t library;
	dynamic_function_t *funcs;
	struct watch_node *next;
} *watch_node_t;

/* root link of the watch_node list */
watch_node_t watch_root;

/* add a node to the watch_node list */
void add_node(int watch, library_t library, dynamic_function_t *funcs) {
	watch_node_t new_node = malloc(sizeof(*new_node));
	new_node->watch = watch;
	new_node->library = library;
	new_node->funcs = funcs;
	new_node->next = watch_root;
	watch_root = new_node;
}

/* delete a node from the watch list */
int delete_node() {
	if(watch_root == NULL) {
		return -1;
	}
	int hold = watch_root->watch;
	watch_root = watch_root->next;
	return hold;
}

/* given a file, find the library and list of dynamic functions 
 * that coorespond to it */
void lookup_watch(char *file, library_t *library, dynamic_function_t **funcs) {
	/* cycle through all watch nodes */
	watch_node_t node = watch_root;
	while(node != NULL) {
		/* compare the file to the library's name */
		if(!strcmp(file, get_file(node->library->name))) {
			*library = node->library;
			*funcs   = node->funcs;
			return;
		}
		node = node->next;
	}
	*library = NULL;
	*funcs   = NULL;
}

/* information about the inotify watcher and its thread */
int inotify_fd;
pthread_t inotify_thread;
int run;

/**
 * watcher thread
 *
 * reads in inotify changes, compares them to the list of watched
 * directories, and reloads the libraries that it finds
 */
void *watcher_thread(void *data) {
	char buffer[BUFFER_SIZE];
	int length;
	int i;
	struct inotify_event *event;
	library_t library;
	dynamic_function_t *funcs;
	int *run = (int*)data;

	while(*run) {
		/* read in inotify events */
		if((length = read(inotify_fd, buffer, BUFFER_SIZE)) < 0) {
			perror("read");
		}
		i = 0;
		/* go through the events */
		while(i < length) {
			event = (struct inotify_event*)(buffer+i);
			if(!(event->mask & IN_IGNORED)) {
				/* lookup the library */
				lookup_watch(event->name, &library, &funcs);
				if(library) {
					/* if it exists, reload it */
					reload_library(library, funcs);
				}
			}
			i += EVENT_SIZE + event->len;
		}
	}
}

/* starts the watcher */
void initialize_watcher() {
	/* create an inotify descriptor */
	if((inotify_fd = inotify_init()) < 0) {
		perror("inotify_init");
	}
	/* start the thread */
	run = 1;
	pthread_create(&inotify_thread, NULL, watcher_thread, &run);
}

/* tells the watcher to watch a given library */
void watch_library(library_t library, dynamic_function_t *funcs) {
	char *dir = get_directory(library->name);
	int watch = inotify_add_watch(inotify_fd, dir, IN_MOVED_TO);
	add_node(watch, library, funcs);
	free(dir);
}

/* stop the watcher thread */
void stop_watcher() {
	run = 0;
	int watch;
	/* first, delete all the inotify nodes */
	while((watch = delete_node()) > 0) {
		inotify_rm_watch(inotify_fd, watch);
	}
	/* close the inotify descriptor */
	close(inotify_fd);
	/* and join with the watcher thread */
	pthread_join(inotify_thread, NULL);
}
