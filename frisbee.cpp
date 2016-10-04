using namespace std;

// An OpenGL Keyboard and Mouse Interaction Program

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>//for cout
#include <math.h>//for math functions
#include <vector>//for triangles
#include "singly.cpp"//homemade linkedlist, not very generic though

// These are defined in a global scope

GLubyte red, green, blue;
int COLORS_DEFINED;

singly linkedList;//list of points
singly linkedPolygonList;//for filled polygon after tesselation
vector<triangle> triangles;//vector of triangle
struct vertex *lastPoint=NULL;//these 2 ptrs help with tesselation
struct vertex *twoPointsAgo=NULL;

float scaledx;
float scaledy;
int scaledMAXWindowx=500;
int scaledMAXWindowy=500;
// Specity the values to place and size the window on the screen

const int WINDOW_POSITION_X = 100;
const int WINDOW_POSITION_Y = 100;
const int WINDOW_MAX_X = 700;
const int WINDOW_MAX_Y = 700;

// Specify the coordinate ranges for the world coordinates in the 2D Frame

const float WORLD_COORDINATE_MIN_X = 000.0;
const float WORLD_COORDINATE_MAX_X = 0700.0;
const float WORLD_COORDINATE_MIN_Y = 000.0;
const float WORLD_COORDINATE_MAX_Y = 0700.0;


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
      glPointSize(15.0);

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
     

 }
//dot product function for 2 line segments that share an endpoint
GLfloat dotProduct(struct vertex *point1, struct vertex *point2, struct vertex *point3)
{
	if(point1==NULL || point2==NULL)
		return 0;
	
	GLfloat line1x, line1y, line1z, line2x, line2y, line2z, dotProduct;
	
	line1x = (point1->x) - (point2->x);
	line1y = (point1->y) - (point2->y);
	line1z = 0.0;

	line2x = (point3->x) - (point2->x);
	line2y = (point3->y) - (point2->y);
	line2z = 0.0;

	dotProduct = (line1x * line2x) + (line2y * line1y) + (line1z * line2z);;

	return dotProduct;
}
//to find the magnitude of a vector
GLfloat magnitudeOf(vertex *point1, vertex *point2)
{
	GLfloat lineX = (point1->x) - (point2->x);
	GLfloat lineY = (point1->y) - (point2->y);
	return sqrt (	pow(lineX, 2.0) + pow(lineY, 2.0)  );
}
//using dot product and magnitude to find angle between 2 vectors
GLfloat angleBetween(vertex *p1, vertex *p2, vertex *p3)
{
	GLfloat cosTheta, theta;
	cosTheta = dotProduct(p1,p2,p3)/(  magnitudeOf(p1,p2) * magnitudeOf(p2,p3)  );
	theta = acos(cosTheta);

	return theta;
}
//uses algorithm from class to determine if two line segments intersect
bool intersect(vertex *point1, vertex *point2, vertex *point3, vertex *point4)
{
	bool intersects = false;

	GLfloat ua, ub, x1, x2, x3, x4, y1, y2, y3, y4, numerator_a, numerator_b, denominator;

	x1 = point1->x; y1 = point1->y;
	x2 = point2->x; y2 = point2->y;
	x3 = point3->x; y3 = point3->y;
	x4 = point4->x; y4 = point4->y;
	
	numerator_a = ((x3-x1) * -(y4-y3)) - (-(x4-x3) * (y3-y1));
	numerator_b = ((x2-x1) *  (y3-y1)) - ( (x3-x1) * (y2-y1));
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


//does the above function for all lines in list
bool noIntersects(singly linkedlist,vertex *lastVertex, vertex *newVertex)
{
	if (linkedlist.getLength() < 3)
		return true;

	struct vertex *headChecker = linkedlist.head;
	struct vertex *anotherLineVertex1=linkedlist.head;
	struct vertex *anotherLineVertex2=linkedlist.head->next;
	while (anotherLineVertex2!=NULL)
	{
		if(intersect(lastVertex, newVertex, anotherLineVertex1, anotherLineVertex2) || 
		   intersect(newVertex, headChecker,anotherLineVertex1, anotherLineVertex2))	
			return false;

		anotherLineVertex1=anotherLineVertex2;
		anotherLineVertex2=anotherLineVertex2->next;
	}

	return true;
}
//a check for if a line exists in a polygon
int numIntersects4Interior(singly linkedlist,GLfloat vertexX, GLfloat vertexY)
{
	int county = 0;
	struct vertex *midpoint = linkedList.createVertex(vertexX, vertexY);
	struct vertex *boundaryPoint = linkedList.createVertex(WINDOW_MAX_X, vertexY);
	struct vertex *anotherLineVertex1=linkedlist.head;
	struct vertex *anotherLineVertex2=linkedlist.head->next;
	while (anotherLineVertex2!=NULL)
	{
		if(intersect(midpoint, boundaryPoint, anotherLineVertex1, anotherLineVertex2))	{
			county++;
			cout<<county<<endl;
		}

		anotherLineVertex1=anotherLineVertex2;
		anotherLineVertex2=anotherLineVertex2->next;
	}
	cout<<"county is "<< county <<endl;
	return county;
}
//checks if proposed line will exists in side polygon by checking how many times it intersects as it goes to the edge of the window
bool inThatThang(vertex *p1, vertex *p2)
{

	GLfloat line1x=p1->x, line1y=p1->y;
	GLfloat line2x=p2->x, line2y=p2->y;

	GLfloat middyBoyX = (line1x + line2x)/2.0; 
	GLfloat middyBoyY= (line1y + line2y)/2.0;
	
	int crossy = numIntersects4Interior(linkedList, middyBoyX, middyBoyY);

	if (crossy%2==1)//if it crosses an odd number of lines, it will exist inside the polygon
		return true;//this is good
	else
		return false;//no bueno
}
//crossproduct function for 2 line segments that share a point
GLfloat crossProduct(struct vertex *point1, struct vertex *point2, struct vertex *point3)
{
	if(point1==NULL || point2==NULL)
		return 0;
	
	GLfloat line1x, line1y, line1z, line2x, line2y, line2z, crossProductZcomponent;
	
	line1x = (point1->x) - (point2->x);
	line1y = (point1->y) - (point2->y);
	line1z = 0.0;

	line2x = (point3->x) - (point2->x);
	line2y = (point3->y) - (point2->y);
	line2z = 0.0;

	crossProductZcomponent = (line1x * line2y) - (line2x * line1y);

	return crossProductZcomponent;
}
//

void drawBox( int x, int y )
{
    typedef GLfloat point[2];     
    point p;

    glColor3ub( red, green, blue );

    // Added code to print window coordinates and world coordinates
    
  //  printf ("%d   %d (window coordinates) ", x, WINDOW_MAX_Y - y );

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

    vertex *newVertex = linkedList.createVertex(p[0],p[1]);//create a vertex from mouse coords when clicked
    
    if(noIntersects(linkedList, lastPoint, newVertex))//if neither the line from this point to the previous point NOR the line from this point to the head
    {//intersect any other point
    	linkedList.append(newVertex); //add that point to the list
	    //draw the point
	    glBegin(GL_POINTS);
		    glVertex2fv(p); 
	    glEnd();
	    glFlush();
		
	    //update most recent points
	    twoPointsAgo=lastPoint;
	    lastPoint=newVertex;
    }
    else//if point will cause an intersection
    {
	    cout << "New point not added. Points may only be added such that it creates no intersecting lines if we connect the dots" << endl;
    }
}
//to draw a filled, untesselated polygon
void makeThePolygonSucka()
{//loop through list of points and feed it to the glBegin
	struct vertex *pointy = linkedList.head;
	glBegin(GL_POLYGON);
		while(pointy!=NULL)
		{
			glVertex2f(pointy->x, pointy->y);
			pointy = pointy->next;
		}
	glEnd();
	glFlush();
}
//post tesselation polygon goodness
void theresANewPolygonInTown()
{//loop through vector of triangle structs and feed each point to glBegin as TRIANGLES
	vector<triangle>::iterator it;

	glBegin(GL_TRIANGLES);
		for(it=triangles.begin(); it!=triangles.end(); ++it)
		{
			vertex *p1=it->p1; vertex *p2=it->p2; vertex *p3=it->p3;
		
			glVertex2f(p1->x, p1->y);
			glVertex2f(p2->x, p2->y);
			glVertex2f(p3->x, p3->y);
		}
	glEnd();
	glFlush();
}
//clear the screen
void whiteItOut()
{
	glColor3f(1.0, 1.0, 1.0);
	makeThePolygonSucka();
	glColor3f(1.0, 0.0, 0.0);
}
//to connect the dots in the order they were added
void lineEmUpSucka()
{
	//kick you out if you try to connect just 1 point
	if (linkedList.head==NULL || linkedList.head->next==NULL)
	{
		cout << "can't draw lines with fewer than 2 points" << endl;
		exit(0);
	}
	struct vertex *firstPoint=linkedList.head;
	struct vertex *secondPoint=linkedList.head->next;
	//loop through list and connect each point one by one in the order it was put in list
	while (secondPoint!=NULL)
	{
		glBegin(GL_LINES);
			glVertex2f(firstPoint->x, firstPoint->y);
			glVertex2f(secondPoint->x, secondPoint->y);
		glEnd();
		firstPoint=secondPoint;
		secondPoint=secondPoint->next;
	}
	//connect final point to head
	glBegin(GL_LINES);
		glVertex2f(firstPoint->x, firstPoint->y);
		glVertex2f(linkedList.head->x, linkedList.head->y);
	glEnd();
	glFlush();
}
//sexy recursive tesselator function
void commenceTesselation(singly linkedlist, struct vertex *p1, struct vertex *p2, struct vertex *p3)
{
	
	if (p1==NULL || p2 ==NULL || p3==NULL)//basically an error check
		return;
	if(crossProduct(p1, p2, p3) == 0.0)//if the 3 points we're working on are colinear, delete the second point and get out
	{
		linkedlist.deleteVertex(p2);
		return;
	}
	else if(crossProduct(p1, p2, p3) < 0.0 && 
		noIntersects(linkedlist, p1, p3) && inThatThang(p1, p3)) //if 3 points are countersclockwise, don't intersect anything if triangulated, and
	{//will draw a line within the shape
		//draw it
		glBegin(GL_LINES);
			glVertex2f(p1->x, p1->y);
			glVertex2f(p3->x, p3->y);
		glEnd();
		glFlush();
		//add a triangle struct of these points to the vector
		struct triangle TDawg = {p1, p2, p3};
		triangles.push_back(TDawg);

		//delete point from list	
		linkedlist.deleteVertex(p2);

		return;
	}
	else//if it fails the tests
	{//MOVE THE VERTICES DOWN THE LIST
		p1=p2;		
		p2=p3;	
		if (p3==linkedlist.last()){
			p3=linkedlist.head;	
		}
		else{
			p3=p3->next;
		}
		//recursion incoming w/ new points
		commenceTesselation(linkedlist, p1, p2 , p3);
	}
}
void tesselateItSucka()
{//starts tesselation
	//grab first 3 points
	struct vertex *p1 = linkedList.head;
	struct vertex *p2 = p1->next;
	struct vertex *p3 = p2->next;
	
	//draw outline of shape
	lineEmUpSucka();
	if (linkedList.getLength() < 3)//less than 3 points, do nothing
		return;
	if (linkedList.getLength()==3)//equal to 3 points, already a triangle; add it to vector and leave
	{
		struct triangle TDawg = {p1,p2,p3};
		triangles.push_back(TDawg);
		return;
	}
	else//4 points or more
	{
		
		while (linkedList.getLength() >= 3)//tessy loop
		{
			if (linkedList.getLength()==3)//when the list is deleted down to 3 vertices, add that triangle to vector and leave
			{
				p1=linkedList.head; p2=p1->next; p3=p2->next;
				struct triangle TDawg = {p1, p2, p3};
				triangles.push_back(TDawg);

				break;
			}
			if (linkedList.getLength() == 4 && crossProduct(p1,p2,p3)>0.0)//if 4 sided and clockwise
			{
				vertex *p4 = p3->next;

				if (angleBetween(p2,p3,linkedList.head) > angleBetween(p2,p3,p3->next))//if line will not exist within polygon
				{//draw other line in quadrilateral, delete vertex from list, add triangle to vector; rinse repeat for following

					glBegin(GL_LINES);
						glVertex2f(p2->x, p2->y);
						glVertex2f(p4->x, p4->y);
					glEnd();
					glFlush();
					
					linkedList.deleteVertex(p3);

					struct triangle TDawg = {p2, p3, p4};
					struct triangle TShizzle = {p1, p2, p4};
					triangles.push_back(TDawg);
					triangles.push_back(TShizzle);
					break;
				}
				else
				{
					glBegin(GL_LINES);
						glVertex2f(p1->x, p1->y);
						glVertex2f(p3->x, p3->y);
					glEnd();
					glFlush();
	
					linkedList.deleteVertex(p2);
					
					struct triangle TDawg = {p1, p2, p3};
					struct triangle TShizzle = {p1, p3, p4};
					triangles.push_back(TShizzle);
					triangles.push_back(TDawg);
					break;
				}
			}
			else if (linkedList.getLength() == 4 && crossProduct(p1,p2,p3)<0.0)//if 4 sided and counterclockwise
			{
				vertex *p4 = p3->next;

				if(angleBetween(p2,p3,linkedList.head) < angleBetween(p2,p3,p3->next))
				{
					glBegin(GL_LINES);
						glVertex2f(p1->x, p1->y);
						glVertex2f(p3->x, p3->y);
					glEnd();
					glFlush();

					struct triangle TDawg = {p1, p2, p3};
					struct triangle TShizzle = {p1, p3, p4};
					triangles.push_back(TShizzle);
					triangles.push_back(TDawg);
					break;
				}
				else
				{
					vertex *p4 = linkedList.last();
					glBegin(GL_LINES);
						glVertex2f(p2->x, p2->y);
						glVertex2f(p4->x, p4->y);
					glEnd();
					glFlush();

					struct triangle TDawg = {p2, p3, p4};
					struct triangle TShizzle = {p1, p2, p4};
					triangles.push_back(TDawg);
					triangles.push_back(TShizzle);
					break;
				}
			}
			else
			{
					//more than 4 sides, get to the sexy stuff
					commenceTesselation(linkedList, p1, p2, p3);	
					//reset points to first 3after every run through
					p1 = linkedList.head;
					p2 = p1->next;
					p3 = p2->next;
			}
			//reset points 
			p1 = linkedList.head;
			p2 = p1->next;
			p3 = p2->next;
			//for safety i suppose
			struct triangle TDawg = {p1,p2,p3};
			triangles.push_back(TDawg);
		}
	}
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
	whiteItOut();
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
  if ( key == 'c' || key == 'C') //to start fresh
  {	
	clearBox();
	linkedList.deleteTheWholeDamnThing();
	lastPoint=NULL;
	twoPointsAgo=NULL;
	triangles.clear();
  }
  if ( key == 'l' || key == 'L')//to clear and draw outline
  {
	  whiteItOut();
	  lineEmUpSucka();
  }
  if ( key == 'p' || key == 'P' || key == 'f' || key == 'F')
  {
	  whiteItOut();
	  makeThePolygonSucka();
  }
  if ( key == 't' || key == 'T')
  {
	  whiteItOut();
	  tesselateItSucka();
  }
  if ( key == 'm' || key == 'M')
	  theresANewPolygonInTown();
}


int main(int argc, char** argv)
{

    myglutInit(argc,argv); /* Set up Window */
    myInit(); /* set attributes */

    // Now start the standard OpenGL glut callbacks //
    
    glutMouseFunc(mouse);  /* Define Mouse Handler */
    glutKeyboardFunc(keyboard); /* Define Keyboard Handler */
    glutDisplayFunc(display); /* Display callback invoked when window opened */
    glutMainLoop(); /* enter event loop */
}

