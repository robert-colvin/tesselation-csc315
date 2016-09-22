using namespace std;

// An OpenGL Keyboard and Mouse Interaction Program

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
//struct for points
struct vertex
{
	GLfloat x;
	GLfloat y;
	int w;
	vertex *next;
}*head;

//class for linkedlist with create, append, and delete
//-----------------------------------------------------------------------------
class singly
{
	public:
		vertex* createVertex(GLfloat xCoord, GLfloat yCoord);
		void append(vertex *node);
		void deleteVertex(vertex *node);
		void deleteTheWholeDamnThing();
//		int findVertex();
		vertex *head;
		singly()
		{
			head=NULL;
		}
};

vertex* singly::createVertex(GLfloat xCoord, GLfloat yCoord)
{
	struct vertex *temp;
	temp = new(struct vertex);
	if(temp==NULL)
	{
		cout << "catastrophe in creation. line 38" << endl;
		return 0;
	}
	else
	{
		temp->x=xCoord;
		temp->y=yCoord;
		temp->next=NULL;
		return temp;
	}
}
void singly::append(vertex *node)
{
	struct vertex *tempStart;
	tempStart=head;
	if(head==NULL)
	{//if list is empty aka this is first entry
		head=node;
		head->next=NULL;
		cout << "NEW HEAD IS " << node->x << " " << node->y << endl;
	}
	else
	{//otherwise...
		while(tempStart->next != NULL)//until you get to last node
		{//move down the list one by one
			 tempStart=tempStart->next;
		}//when you get to last node...
		//ensure new node's next is NULL to be safe or something
		node->next=NULL;
		//append new node to last node in list
		tempStart->next=node;
		cout << "APPENDED " << node->x << " " << node->y << endl;//announce it for the world to see
	}
}

void singly::deleteVertex(vertex *node)
{
	struct vertex *temp=head;
	if (head==node)
	{
		if(head->next!=NULL)
		{
			temp = head;
			head=head->next;
			delete temp;
			delete node;
		}
		else if(head->next==NULL)
			head=NULL;
	}
	else
	{
		while(temp->next != node)
		{
			if(temp->next==NULL)
			{
				cout << "error in search for deletion. 91" << endl;
			}
			temp=temp->next;
		}
		temp->next=temp->next->next;
		delete node;
		return;
	}
}
void singly::deleteTheWholeDamnThing()
{
	vertex *deadManWalking=head;
	while(head!=NULL)
	{
		head=head->next;
		deadManWalking=NULL;
		delete deadManWalking;
	}

}
//CLASS END
//----------------------------------------------------------------------

// These are defined in a global scope

GLubyte red, green, blue;
int COLORS_DEFINED;

singly linkedList;
struct vertex *lastPoint=NULL;

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


GLfloat crossProduct(struct vertex *point1, struct vertex *point2)
{
	if(point1==NULL)
		return 0;
	
	GLfloat x1 = point1->x, y1 = point1->y;
	GLfloat x2 = point2->x, y2 = point1->y;
	GLfloat crossProduct;

	crossProduct = (x1 * y2) - (x2 * y1);

	return crossProduct;
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
    
    if(crossProduct(lastPoint, newVertex) <= 0.0)
    {
    	linkedList.append(newVertex); 
   
  	  printf ("\t  %f   %f (world coordinates) \n", p[0], p[1] );

	    glBegin(GL_POINTS);
		    glVertex2fv(p); 
	    glEnd();
	    glFlush();
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

