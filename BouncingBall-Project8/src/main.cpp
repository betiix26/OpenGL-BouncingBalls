#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <stdlib.h>   
#include <GL/glut.h>
#endif

#include <cmath>
#include "header.h"
#include <vector>
#include <vec3.hpp>

//colors
GLfloat WHITE[] = { 1, 1, 1 };
GLfloat RED[] = { 1, 0, 0 };
GLfloat YELLOW[] = { 1, 1, 0 };
GLfloat GREEN[] = { 0, 1, 0 };
GLfloat BLUE[] = { 0, 0, 1 };
GLfloat MAGENTA[] = { 1, 0, 1 };
GLfloat PINK[] = { 0.5, 0, 0.5 };
GLfloat CYAN[] = { 0, 1, 1 };
GLfloat BROWN[] = { 0.30, 0.15, 0.05 };

//const double FRAME_RATE_MS = 600.0 / 60.0;

//camera
class Camera {
	double theta;	   //pozitiile x si z 
	double y;		  //y curent
	double dTheta;   //incrementeaza in theta pentru a roti camera
	double dy;      //incerementeaza in y pt a misca camera sus-jos
public:
	Camera() : theta(0), y(15), dTheta(0.04), dy(0.2) {}
	double getX()
	{
		return 15 * cos(theta); //era 10
	}
	double getY()
	{
		return y;
	}
	double getZ() {
		return 10 * sin(theta); 
	}
	void moveRight()
	{
		theta += dTheta;
	}

	void moveLeft()
	{
		theta -= dTheta;
	}
	void moveUp()
	{
		y += dy;
	}
	void moveDown()
	{
		if (y > dy)
			y -= dy;
	}
};

class Ball
{
	double radius;
	GLfloat* color;
	double maximumHeight;
	double x;
	double y;
	double z;
	int direction;
public:
	Ball(double r, GLfloat* c, double h, double x, double z) :
		radius(r), color(c), maximumHeight(h), direction(-1), y(h), x(x), z(z) {}

	void update() {
		y += direction * 0.05;
		if (y > maximumHeight) {
			y = maximumHeight;
			direction = -1;
		}
		else if (y < radius)
		{
			y = radius;
			direction = 1;
		}
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
		glTranslated(x, y, z);
		glutSolidSphere(radius, 30, 30);
		glPopMatrix();
	}
};

class Board
{
	int displayListId;
	int width;
	int depth;
	GLfloat* color;
public:

	Board(int width, int depth, GLfloat* c) : width(width), depth(depth), color(c) {}
	double centerX() { return width / 2; }
	double centerZ() { return depth / 2; }

	void create()
	{
		displayListId = glGenLists(1);
		glNewList(displayListId, GL_COMPILE);
		glBegin(GL_QUADS);
		glNormal3d(0, 1, 0);
		for (int x = 0; x < width - 1; x++)
		{
			for (int z = 0; z < depth - 1; z++) {
				glGetMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, (x + z) % 2 == 0 ? BLUE : WHITE);
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
				glVertex3d(x, 0, z);
				glVertex3d(x + 1, 0, z);
				glVertex3d(x + 1, 0, z + 1);
				glVertex3d(x, 0, z + 1);
			}
		}
		glEnd();
		glEndList();
	}

	void draw()
	{
		glCallList(displayListId);
	}

};


Board board(9, 9, BROWN);
Camera camera;
Ball balls[] = {
	Ball(0.5, YELLOW, 6, 4, 1),
	Ball(0.5, BLUE, 6, 3, 4),
	Ball(0.5, GREEN, 6, 2, 7)
};


void init() {
	//glClearColor(0.6f, 0.8f, 1.0f, 1.0f); //fundal albastru 
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
	glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, WHITE);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	board.create();
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
		board.centerX(), 0.0, board.centerZ(),
		0.0, 1.0, 0.0);
	board.draw();
	for (int i = 0; i < sizeof balls / sizeof(Ball); i++)
	{
		balls[i].update();
	}
	glFlush();
	
	GLfloat* color = PINK;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

	//perete fata
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.0f, 0.0f);    //stanga-sus
	glVertex3f(0.0f, 8.0f, 8.0f);    //stanga-jos
	glVertex3f(0.0f, 0.0f, 8.0f);    //dreapta-jos
	glVertex3f(0.0f, 8.0f, 0.0f);    //dreapta-sus
	glEnd();

	//perete dreapta
	glBegin(GL_QUADS);
	glVertex3f(8.0f, 0.0f, 0.0f);    //dreapta-sus
	glVertex3f(8.0f, 8.0f, 0.0f);    //dreapta-jos
	glVertex3f(0.0f, 0.0f, 0.0f);    //stanga-jos
	glVertex3f(0.0f, 8.0f, 0.0f);    //stanga-sus
	glEnd();

	//perete stanga
	glBegin(GL_QUADS);
	glVertex3f(8.0f, 0.0f, 8.0f);    //dreapta-sus
	glVertex3f(0.0f, 8.0f, 8.0f);    //dreapta-jos
	glVertex3f(0.0f, 0.0f, 8.0f);    //stanga-jos
	glVertex3f(8.0f, 8.0f, 8.0f);    //stanga-sus
	glEnd();

	glutSwapBuffers();
}

//creeaza o camera care are aceeasi dimensiune cu window-ul
void reshape(GLint width, GLint height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, GLfloat(width) / GLfloat(height), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}

//draw next frame
void timer(int unused)
{
	glutPostRedisplay();
	glutTimerFunc(FRAME_RATE_MS, timer, unused); //balls's speed
}

//misca camera in functie de tasta apasata
void special(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: camera.moveLeft(); break;
	case GLUT_KEY_RIGHT: camera.moveRight(); break;
	case GLUT_KEY_UP: camera.moveUp(); break;
	case GLUT_KEY_DOWN: camera.moveDown(); break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Bouncing Balls");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutTimerFunc(FRAME_RATE_MS, timer, 0);
	init();
	glutMainLoop();
}
