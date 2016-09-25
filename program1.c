

// An OpenGL Keyboard and Mouse Interaction Program

#include <GL/glut.h>
#include <stdio.h>

//struct for points
typedef struct vertex{
	GLfloat x;
	GLfloat y;
	int w;
	struct vertex *next;
}vertex;

// These are defined in a global scope

GLubyte red, green, blue;
int COLORS_DEFINED;
vertex *head = NULL, *last = NULL;

// Specity the values to place and size the window on the screen
//mouse coords
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


vertex* makeVertex(GLfloat xCoord, GLfloat yCoord)
{printf("\n106");
	vertex newVertex;
printf("\n107");
	//vertex *newVertPtr = malloc(sizeof(newVert));	printf("\n107");
	/*if(newVert!=NULL)
	{printf("\n109");*/
		newVertex.x=xCoord;
		newVertex.y=yCoord;
		printf("\n113 before");newVertex.next=NULL;printf("\n113");
		vertex* newVert=&newVertex;
		return newVert;
//	}
//	else
//	{
//		printf("\nPhenomenal failure on node creation");
//		return 0;
//	}
}
void addVertex(vertex *newVertex)
{
	if(head==last && last==NULL)
	{printf("\n124");
		head=last=newVertex;
		head->next=NULL;
		last->next=NULL;
		printf("\nNode at head");
	}
	else
	{
		last->next=newVertex;
		last=newVertex;
		last->next=NULL;
		printf("\nnode at end of list");
	}
}

void drawBox( int x, int y )
{
 //   vertex *thisPoint = NULL;
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
//geufbujs

    p[1] = WORLD_COORDINATE_MIN_Y +  p[1] / WINDOW_MAX_Y * 
                                    (WORLD_COORDINATE_MAX_Y - WORLD_COORDINATE_MIN_Y);
/*
    if (headPtr==NULL)
    {
	    headPtr=malloc(sizeof(headPtr));
	    headPtr->x=p[0];
	    headPtr->y=p[1];
	    headPtr->next=NULL;
    	    printf ("\t  %f   %f (coords in node) \n", headPtr->x, headPtr->y );
    }
    else
    {
	    vertex *nextOne=NULL;
	    nextOne=malloc(sizeof(nextOne));
	    nextOne->x=p[0];
	    nextOne->y=p[1];
	    nextOne->next=NULL;
    	    printf ("\t  %f   %f (head is not null) \n", nextOne->x, nextOne->y );
	    head->next=nextOne;
    }
*/
  //  thisPoint=makeVertex(p[0], p[1]);
printf("\n188");    addVertex(makeVertex(p[0],p[1]));	printf("\n188after");
    
    printf ("\t  %f   %f (world coordinates) \n", p[0], p[1] );

    glBegin(GL_POINTS);
    glVertex2fv(p); 
    glEnd();
    glFlush();
}

void lineEmUp(void)
{
/*    typedef GLfloat point[2];     
    point p;

    glColor3f( 255.0, 0.0, 0.0 );

    p[0] = x;
    p[1] = WINDOW_MAX_Y-y;

      
    p[0] = WORLD_COORDINATE_MIN_X +  p[0] / WINDOW_MAX_X * 
                                    (WORLD_COORDINATE_MAX_X - WORLD_COORDINATE_MIN_X);


    p[1] = WORLD_COORDINATE_MIN_Y +  p[1] / WINDOW_MAX_Y * 
                                    (WORLD_COORDINATE_MAX_Y - WORLD_COORDINATE_MIN_Y);
*/
//MAKE THESE LINES-----------------------------------------------------------------------------------------------------------------------------
    //glLineWidth(2.5);
    //glColor3ub(red, green, blue);
    glBegin(GL_LINES);
    	glVertex2f(10, 10);
    	glVertex2f(200, 200);
    glEnd();
    glFlush();
//-------------------------------------------------------------------------------------------------------------------------
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
        drawBox( x, y);
     }

  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
     {
        //printf ("%d   %d\n", x, y);
        lineEmUp();
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
}


int main(int argc, char** argv)
{
    head=malloc(sizeof(head));
//    head=NULL;
    myglutInit(argc,argv); /* Set up Window */
    myInit(); /* set attributes */

    // Now start the standard OpenGL glut callbacks //
    
    glutMouseFunc(mouse);  /* Define Mouse Handler */
    glutKeyboardFunc(keyboard); /* Define Keyboard Handler */
    glutDisplayFunc(display); /* Display callback invoked when window opened */
    glutMainLoop(); /* enter event loop */
}

