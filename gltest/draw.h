#ifndef DRAW_H
#define DRAW_H

#include <dyload.h>

#ifdef DYNAMIC
	void setup_library_draw();
	extern void (*draw)(void);
#else
	void draw(void);
#endif

#endif /* DRAW_H */
