#include <GL/glfw.h>
#include <math.h>
#include <stdio.h>

void draw_circle(float radius, int points) {
	int i;
	glBegin(GL_LINE_LOOP);
	for (i=0; i<points; ++i) {
		glVertex2f(radius*cos(i*6.282/points), radius*sin(i*6.282/points));
	}
	glEnd();
}

void filled_circle(float in_rad, float out_rad, int points) {
	int i;
	glBegin(GL_TRIANGLE_STRIP);
	for (i=0; i<points+1; ++i) {
		float ang = i*M_PI*2/points;
		glVertex2f(in_rad*cos(ang), in_rad*sin(ang));
		glVertex2f(out_rad*cos(ang), out_rad*sin(ang));
	}
	glEnd();
}

void gear(float in_rad, float out_rad, int spokes, int points) {
	int i;
	for (i=0; i<spokes; ++i) {
		float ang = i*2*M_PI/spokes;
		glBegin(GL_TRIANGLE_STRIP);
		glVertex2f(in_rad*cos(ang), in_rad*sin(ang));
		glVertex2f(out_rad*cos(ang+0.04), out_rad*sin(ang+0.04));
		glVertex2f(in_rad*cos(ang+0.16), in_rad*sin(ang+0.16));
		glVertex2f(out_rad*cos(ang+0.12), out_rad*sin(ang+0.12));
		glEnd();
	}
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	static float angle = 0;

	int i;
	angle += 0.2;

	glPushMatrix();
		for (i=1; i<7; ++i) {
			glRotatef(angle*(i+3)/15.0, 0, 0, 1);
			glColor3f((i+2)/9.0, (i+2)/9.0, (i+2)/9.0);
			filled_circle(i/3.05, i/2.9, 80);
			gear(i/2.9, i/2.8, 18, 80);
		}
	glPopMatrix();

	glPushMatrix();
		glScalef(2, 2, 2);

		glBegin(GL_TRIANGLE_STRIP);
			glColor3f(0.2, 0.2, 0.8);
			glVertex2f(1, 0);
			glVertex2f(0, 1);
			glColor3f(0.05, 0.05, 0.2);
			glVertex2f(0.05, -0.05);
			glVertex2f(-0.05, 0.05);
			glColor3f(0.2, 0.2, 0.8);
			glVertex2f(0, -1);
			glVertex2f(-1, 0);
		glEnd();
	glPopMatrix();
}
