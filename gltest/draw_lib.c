#include "draw.h"

void (*draw)(void);

INIT_LIBRARY(draw, 1, FUNC(draw));

void setup_library_draw() {
	SETUP_LIBRARY(draw, "./draw.so");
}
