#include <GL/glut.h>

struct vertex
{
	GLfloat x;
	GLfloat y;
	int w;
	vertex *next;
};
struct triangle
{
	vertex *p1;
	vertex *p2;
	vertex *p3;
	triangle *next;
};
