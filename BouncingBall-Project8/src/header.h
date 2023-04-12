#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>

//extern const double FRAME_RATE_MS;
const double FRAME_RATE_MS = 600.0 / 60.0;

void update() {};
void create() {};
void draw() {};
void init();
void display();
void reshape(GLint w, GLint h);
void timer(int v);
void special(int key, int, int);
int main(int argc, char** argv);

void moveRight() {};
void moveLeft() {};
void moveUp() {};
void moveDown() {};