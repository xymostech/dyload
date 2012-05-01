#ifndef DRAW_H
#define DRAW_H

#include "watch_library.h"
#include "library.h"

void setup_library_draw();
#ifdef DYNAMIC
	extern void (*draw)(void);
#else
	void draw(void);
#endif

#endif /* DRAW_H */
