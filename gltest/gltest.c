#include <stdio.h>
#include <GL/glfw.h>

#include "watch_library.h"

#include "draw.h"

void GLFWCALL set_window_size(int w, int h) {
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, w, h);
		gluOrtho2D(-w/200.0, w/200.0, -h/200.0, h/200.0);
	glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
}

int main(int argc, char **argv) {
	int running = 1;

#ifdef DYNAMIC
	initialize_watcher();
	setup_library_draw();
#endif

	glfwInit();

	glfwOpenWindow(800, 600, 0, 0, 0, 8, 16, 0, GLFW_WINDOW);
	glfwSetWindowSizeCallback(set_window_size);

	glClearColor(0.1, 0.1, 0.1, 0);

	while(running) {
		glfwSwapBuffers();
#ifdef DYNAMIC
		start_use_library();
#endif
		draw();
#ifdef DYNAMIC
		stop_use_library();
#endif

		running = !glfwGetKey(GLFW_KEY_ESC) &&
		          glfwGetWindowParam(GLFW_OPENED);
		
		glfwSleep(0.01);
	}

	glfwCloseWindow();

	glfwTerminate();

#ifdef DYNAMIC
	stop_watcher();
#endif
}
