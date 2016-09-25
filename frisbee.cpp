using namespace std;

// An OpenGL Keyboard and Mouse Interaction Program

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include "structs.h"
#include "singly.cpp"

// These are defined in a global scope

GLubyte red, green, blue;
int COLORS_DEFINED;

singly linkedList;
struct vertex *lastPoint=NULL;
struct vertex *twoPointsAgo=NULL;

float scaledx;
float scaledy;
int scaledMAXWindowx=500;
int scaledMAXWindowy=500;
// Specity the values to place and size the window on the screen

const int WINDOW_POSITION_X = 100;
const int WINDOW_POSITION_Y = 100;
const int WINDOW_MAX_X = 500;
const int WINDOW_MAX_Y = 500;

// Specify the coordinate ranges for the world coordinates in the 2D Frame

const float WORLD_COORDINATE_MIN_X = 200.0;
const float WORLD_COORDINATE_MAX_X = 2400.0;
const float WORLD_COORDINATE_MIN_Y = 200.0;
const float WORLD_COORDINATE_MAX_Y = 2400.0;


void myglutInit( int argc, char** argv )
{
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); /* default, not needed */
    glutInitWindowSize(WINDOW_MAX_X,WINDOW_MAX_Y); /* set pixel window */
    glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y); /* place window top left on display */
    glutCreateWindow("Mouse and Keyboard Interaction"); /* window title */
}


void myInit(void)
{
 
/* standard OpenGL attributes */

      glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
      glColor3f(1.0, 0.0, 0.0); /* draw in red */
      glPointSize(10.0);

      COLORS_DEFINED = 0;

/* set up viewing window with origin lower left */

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(WORLD_COORDINATE_MIN_X, WORLD_COORDINATE_MAX_X,
                 WORLD_COORDINATE_MIN_Y, WORLD_COORDINATE_MAX_Y);
      glMatrixMode(GL_MODELVIEW);
}
/*
void reshape(int newx, int newy)
{
      float scaledMaxX, scaledMaxY;
      scaledx = (float) newx/(float) WINDOW_MAX_X;
      scaledy = (float) newy/(float) WINDOW_MAX_Y;

      scaledMaxX=WORLD_COORDINATE_MIN_X+scaledx * (WORLD_COORDINATE_MAX_X-WORLD_COORDINATE_MIN_X);

      scaledMaxY=WORLD_COORDINATE_MIN_Y+scaledy * (WORLD_COORDINATE_MAX_Y-WORLD_COORDINATE_MIN_Y);
      
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(WORLD_COORDINATE_MIN_X, scaledMaxX,
                 WORLD_COORDINATE_MIN_Y, scaledMaxY);
      glMatrixMode(GL_MODELVIEW);
      glViewport(0,0,newx, newy);
}
*/

void display( void )
{

/* define a point data type */

    typedef GLfloat point[2];     

    point p; /* A point in 2-D space */

    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    if (!COLORS_DEFINED) {
       red   = 255;
       green = 0; 
       blue  = 0;
    }

    glColor3ub( red, green, blue ); 

    /* define point */

    p[0] = 100; 
    p[1] = 100;
   
    /* plot new point */

        glBegin(GL_POINTS);
            glVertex2fv(p); 
        glEnd();
  
     
     glFlush(); /* clear buffers */

 }

//CROSSPRODUCT WORKS FOR SURE
GLfloat crossProduct(struct vertex *point1, struct vertex *point2, struct vertex *point3)
{
	if(point1==NULL || point2==NULL)
		return 0;
	
	GLfloat line1x, line1y, line1z, line2x, line2y, line2z, crossProduct;
	
	line1x = (point1->x) - (point2->x);
	line1y = (point1->y) - (point2->y);
	line1z = 0.0;

	line2x = (point3->x) - (point2->x);
	line2y = (point3->y) - (point2->y);
	line2z = 0.0;

	crossProduct = (line1x * line2y) - (line2x * line1y);

	return crossProduct;
}
//
bool intersect(vertex *point1, vertex *point2, vertex *point3, vertex *point4)
{
	bool intersects = false;

	GLfloat ua, ub, x1, x2, x3, x4, y1, y2, y3, y4, numerator_a, numerator_b, denominator;

	x1 = point1->x; y1 = point1->y;
	x2 = point2->x; y2 = point2->y;
	x3 = point3->x; y3 = point3->y;
	x4 = point4->x; y4 = point4->y;
	
	numerator_a = ((x3-x1) * -(y4-y3)) - (-(x4-x3) * (y3-y1));
	numerator_b = ((x2-x1) *  (x3-x1)) - ( (x3-x1) * (y2-y1));
	denominator = ((x2-x1) * -(y4-y3)) - (-(x4-x3) * (y2-y1));

	ua = numerator_a/denominator;
	ub = numerator_b/denominator;

	if (0 < ua && ua < 1 &&
	    0 < ub && ub < 1)
	{
		intersects = true;
	}
	return intersects;
}

bool noIntersects(singly linkedlist,vertex *lastVertex, vertex *newVertex)
{
	if (linkedlist.getLength() < 3)
		return true;

	struct vertex *anotherLineVertex1=linkedlist.head;
	struct vertex *anotherLineVertex2=linkedlist.head->next;

	for (int i = 0; i < linkedlist.getLength(); i++)
	{
		if(intersect(lastVertex, newVertex, anotherLineVertex1, anotherLineVertex2))
			return false;
	}
	return true;
}

void drawBox( int x, int y )
{
    typedef GLfloat point[2];     
    point p;

    glColor3ub( red, green, blue );

    // Added code to print window coordinates and world coordinates
    
    printf ("%d   %d (window coordinates) ", x, WINDOW_MAX_Y - y );

    // I switch the mouse coordinate below and...

    p[0] = x;
    p[1] = WINDOW_MAX_Y - y;  

    // then use the scaling equations we talked about in class...

    // Note, since x and y have to be between 0 and WINDOW_MAX_X and WINDOW_MAX_Y respectively, the coding
    // is actually a little simpler. 
    
    p[0] = WORLD_COORDINATE_MIN_X +  p[0] / WINDOW_MAX_X * 
                                    (WORLD_COORDINATE_MAX_X - WORLD_COORDINATE_MIN_X);


    p[1] = WORLD_COORDINATE_MIN_Y +  p[1] / WINDOW_MAX_Y * 
                                    (WORLD_COORDINATE_MAX_Y - WORLD_COORDINATE_MIN_Y);

    vertex *newVertex = linkedList.createVertex(p[0],p[1]);
    
    if(crossProduct(twoPointsAgo, lastPoint, newVertex) <= 0.0 && noIntersects(linkedList, lastPoint, newVertex))
    {
    	linkedList.append(newVertex); 
   
  	  printf ("\t  %f   %f (world coordinates) \n", p[0], p[1] );

	    glBegin(GL_POINTS);
		    glVertex2fv(p); 
	    glEnd();
	    glFlush();
	    twoPointsAgo=lastPoint;
	    lastPoint=newVertex;	
    }
    else
    {
	    cout << "New point not added. Points may only be added in a counterclockwise or linear matter" << endl;
    }
}

void lineEmUpSucka()
{
	if (linkedList.head==NULL || linkedList.head->next==NULL)
	{
		cout << "can't draw lines with fewer than 2 points" << endl;
		exit(0);
	}
	struct vertex *firstPoint=linkedList.head;
	struct vertex *secondPoint=linkedList.head->next;
	
	while (secondPoint!=NULL)
	{
		glBegin(GL_LINES);
			glVertex2f(firstPoint->x, firstPoint->y);
			glVertex2f(secondPoint->x, secondPoint->y);
		glEnd();
		firstPoint=secondPoint;
		secondPoint=secondPoint->next;
	}
	glBegin(GL_LINES);
		glVertex2f(firstPoint->x, firstPoint->y);
		glVertex2f(linkedList.head->x, linkedList.head->y);
	glEnd();
	glFlush();
}

void eraseBox( int x, int y )
{
    typedef GLfloat point[2];     
    point p;

    glColor3f( 1.0, 1.0, 1.0 );

    p[0] = x;
    p[1] = WINDOW_MAX_Y - y;  

    p[0] = WORLD_COORDINATE_MIN_X +  p[0] / WINDOW_MAX_X * 
                                    (WORLD_COORDINATE_MAX_X - WORLD_COORDINATE_MIN_X);


    p[1] = WORLD_COORDINATE_MIN_Y +  p[1] / WINDOW_MAX_Y * 
                                    (WORLD_COORDINATE_MAX_Y - WORLD_COORDINATE_MIN_Y);

    glBegin(GL_POINTS);
      glVertex2fv(p); 
    glEnd();
    glFlush();
}




void clearBox()
{
       glClear(GL_COLOR_BUFFER_BIT); 
       glFlush();
}


void mouse( int button, int state, int x, int y )
{ 

  if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
     {
        //printf ("%d   %d\n", x, y);
        drawBox( x, y );
     }

  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
     {
        //printf ("%d   %d\n", x, y);
        //eraseBox( x, y );
        lineEmUpSucka();
     }
  
  if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
     {
        //printf ("%d   %d\n", x, y);
        clearBox();
     }
}


void keyboard( unsigned char key, int x, int y )
{ 
  if ( key == 'q' || key == 'Q') exit(0);
  if ( key == 'c' || key == 'C') 
  {	
	clearBox();
	linkedList.deleteTheWholeDamnThing();
	lastPoint=NULL;
  }
}


int main(int argc, char** argv)
{

    myglutInit(argc,argv); /* Set up Window */
    myInit(); /* set attributes */

    // Now start the standard OpenGL glut callbacks //
    
    //glutReshapeFunc(reshape);
    glutMouseFunc(mouse);  /* Define Mouse Handler */
    glutKeyboardFunc(keyboard); /* Define Keyboard Handler */
    glutDisplayFunc(display); /* Display callback invoked when window opened */
    glutMainLoop(); /* enter event loop */
}

