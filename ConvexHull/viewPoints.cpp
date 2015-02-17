/* view1.c 

Laura Toma

What it does:  

 - draws a set of points in the default 2D projection and with no

 - transformations.  a tentative function for printing and drawing a
 - list of points (assumed to be a convex hull). These functions were
 - not debigged so use them at your own risk.

*/

#include "geom.h"
#include "rtimer.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>


#include <GLUT/glut.h>

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h> // Header File For The OpenGL32 Library
#include <OpenGL/glu.h> // Header File For The GLu32 Library
#include <GLUT/glut.h> // Header File For The GLut Library

GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};

GLint fillmode = 0;



/* forward declarations of functions */
void display(void);
void keypress(unsigned char key, int x, int y);
void main_menu(int value);



/* global variables */
const int WINDOWSIZE = 500; 

//the array of n points 
point2D*  points = NULL;
int n;  

//a list of points holding the convex hull 
pointNode*  hull = NULL; 



void initialize_points_arrow() {
    assert(points);
    
    double pos = WINDOWSIZE / 4;
    double slope = 0.5;
    double offset = 200;
    int mod = 3;
    
    
    // Draw first third of the arrow.
    for (int i = 0; i < n / mod; i++) {
        switch (i % mod) {
            case 0: {
                // Upper half of head.
                points[i].y = pos + i * slope;
                points[i].x = i + offset;
            } break;
            case 1: {
                // Middle.
                points[i].y = pos;
                points[i].x = i + offset;
            } break;
            case 2: {
                // Bottom half of head.
                points[i].y = pos + i * slope * -1;
                points[i].x = i + offset;
            } break;
            default:
                break;
        }
    }
    
    
    // Fill in the rest with a straight line.
    for (int i = n / mod; i < n; i += mod) {
        points[i].y = pos;
        points[i].x = i + offset;
    }
}


void initialize_points_stuff(){
    assert(points);
    int i;
    points[0].x = 100;
    points[0].y  = 100;
    for(i = 1; i < n/2; i++){
        points[i].y = WINDOWSIZE*2/3-i;
        points[i].x = WINDOWSIZE/2 +i;
    }
    
    for(i = n/2; i < n; i++){
        points[i].x = WINDOWSIZE/3-i;
        points[i].y = (WINDOWSIZE*2/3) + i;
    }
    
    
}
void initialize_points_cascade(){
    assert(points);
    int i;
    points[0].x = WINDOWSIZE/4;
    points[0].y = WINDOWSIZE/4;
    points[1].x = 3*WINDOWSIZE/4;
    points[1].y = WINDOWSIZE/4;
    
    for(i = 2; i < n-1; i++){
        points[i].x = 5*WINDOWSIZE/11 - (int)100*cos(i*M_PI/(2*n));
        points[i].y = (WINDOWSIZE/3) + (int)100*sin(i*M_PI/(2*n));
    }
    
    points[n-1].x = WINDOWSIZE/4;
    points[n-1].y = 3*WINDOWSIZE/4;
}

void initialize_points_rightangle(){
    assert(points);
    int i;
    int j;
    for(i=0; i< n/2; i++){
        points[i].x = WINDOWSIZE/4;
        points[i].y= WINDOWSIZE/4 + (WINDOWSIZE/4)/(n/2)*i;
    }
    for(j=i; j< n; j++){
        points[j].x = WINDOWSIZE/4 +(WINDOWSIZE/4)/(n/2)*(j-i);
        points[j].y = WINDOWSIZE/4 + (WINDOWSIZE/4)/(n/2)*i;
    }
}
void initialize_points_3() {
    assert(points);
    
    int i;
    for (i=0; i<n; i++) {
        if (i%2 == 0) {
            points[i].x = .2*WINDOWSIZE;
            points[i].y =  random() % ((int)(.6*WINDOWSIZE));
            points[i].y += .2*WINDOWSIZE;
        };
        if (i%2 == 1)  {
            points[i].x = random() % ((int)(.6*WINDOWSIZE));
            points[i].x +=  .2*WINDOWSIZE;
            points[i].y = .2*WINDOWSIZE;
        }
    }
}
void initialize_points_ten() {
    int i;
    int upperrad = .3*WINDOWSIZE;
    int lowerrad = .4*WINDOWSIZE;
    int eyesep = .1*WINDOWSIZE;
    int eyeheight = .2*WINDOWSIZE;
    int twoPi = 2*M_PI;
    for (i=0; i<(n/2)-1; i++) {
        points[i].x = WINDOWSIZE/2 + upperrad*cos(twoPi*i);
        points[i].y = WINDOWSIZE/2 - fabs(upperrad*sin(twoPi*i));
    }
    for (i=(n/2)-1; i<n-2; i++) {
        points[i].x = WINDOWSIZE/2 + lowerrad*cos(twoPi*i);
        points[i].y = WINDOWSIZE/2 - fabs(lowerrad*sin(twoPi*i));
    }
    points[n-2].x = WINDOWSIZE/2 - eyesep;
    points[n-2].y = WINDOWSIZE/2 + eyeheight;
    points[n-1].x = WINDOWSIZE/2 + eyesep;
    points[n-1].y = WINDOWSIZE/2 + eyeheight;
    
}
void initialize_points_cubic() {
    //points must be allocated
    assert(points);
    
    
    float step = (cbrt(WINDOWSIZE * WINDOWSIZE) - cbrt(-WINDOWSIZE * WINDOWSIZE)) / n;
    int index = 0;
    
    
    for (float i = cbrt(-WINDOWSIZE * WINDOWSIZE) + WINDOWSIZE / 2; index < n / 2; i += step * 2) {
        points[index].x = (int) i;
        points[index].y = (int) ((i - WINDOWSIZE / 2) * (i - WINDOWSIZE / 2) * (i - WINDOWSIZE / 2) / (WINDOWSIZE * 2) + WINDOWSIZE / 2);
        index++;
    }
    
    
    for (float i = -cbrt(-WINDOWSIZE * WINDOWSIZE) + WINDOWSIZE / 2; index < n; i -= step * 2) {
        points[index].x = (int) i;
        points[index].y = (int) (-(i - WINDOWSIZE / 2) * (i - WINDOWSIZE / 2) * (i - WINDOWSIZE / 2) / (WINDOWSIZE * 2) + WINDOWSIZE / 2);
        index++;
    }
}
void initialize_points_horizontal_line()
{
    assert(points);
    int i;
    for (i = 0; i < n; i++){
        points[i]. x = (int) (.3*WINDOWSIZE) / 2 + random() % ((int)(.7*WINDOWSIZE));
        points[i].y = (int) WINDOWSIZE / 2;
    }
}

void initialize_points_vertical_line()
{
    assert(points);
    int i;
    for(i = 0; i < n; i ++){
        points[i].x = (int) WINDOWSIZE / 2;
        points[i].y = (int) (.3*WINDOWSIZE) / 2 + random() % ((int) (.7*WINDOWSIZE));
    }
}

void init_x_marks_the_spot() {
    assert(points);
    int start_x = WINDOWSIZE/2;
    int start_y = WINDOWSIZE/2;
    int scale = 5;
    for(int i = 0; i < (n/4); i++) {
        points[i].x = start_x + scale*i;
        points[i].y = start_y + scale*i;
    }
    for(int i = (n/4); i < (n/2); i++) {
        points[i].x = start_x - scale*(i-(n/4));
        points[i].y = start_y + scale*(i-(n/4));
    }
    for(int i = (n/2); i < (3*n/4); i++) {
        points[i].x = start_x - scale*(i-(n/2));
        points[i].y = start_y - scale*(i-(n/2));
    }
    for(int i = (3*n/4); i < n; i++) {
        points[i].x = start_x + scale*(i-(3*n/4));
        points[i].y = start_y - scale*(i-(3*n/4));
    }
}

/* ****************************** */
/* initialize  the array of points stored in global variable points[] with random points */
void initialize_points_random() {
  //points must be allocated 
  assert(points); 
  
  int i; 
  for (i=0; i<n; i++) {
    points[i].x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
    points[i].y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
  }
}


/* ****************************** */
/* initialize the array of points stored in global variable points[]
   with random points shaped like a star */
void initialize_points_star() {
  
  //points must be allocated 
  assert(points); 
  
  int i; 
  for (i=0; i<n; i++) {
    if (i%2 == 0) {
      
      points[i].x = (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE)); 
      points[i].y =  random() % ((int)(.7*WINDOWSIZE))  / 5;
      points[i].y += (int)((1-.7/5)*WINDOWSIZE/2);
    };
    if (i%2 == 1)  {
      
      points[i].x = random() % ((int)(.7*WINDOWSIZE)) / 5; 
      points[i].x +=  (int)((1-.7/5)*WINDOWSIZE/2);
      points[i].y =  (int)(.3*WINDOWSIZE)/2 + random() % ((int)(.7*WINDOWSIZE));
    }
   
  }//for i

}

/* ****************************** */
/* print the array of points stored in global variable points[]*/
void print_points() {
  assert(points); 
  int i; 
  printf("points: ");
  for (i=0; i<n; i++) {
    printf("[%3d,%3d] ", points[i].x, points[i].y);
  }
  printf("\n");
  fflush(stdout);  //flush stdout, weird sync happens when using gl thread
}

/* ****************************** */
//print the list of points in global variable  hull; 
void print_hull () {
  
  printf("convex hull: ");
  int i=0;
  pointNode* crt = hull; 
  while (crt) {
    i++; 
    printf("[%3d,%3d] ", crt->p.x, crt->p.y);
    crt = crt->next; 
  }
  printf(" total %d points\n", i);
}



/* ****************************** */
int main(int argc, char** argv) {

  //read number of points from user
  if (argc!=2) {
    printf("usage: viewPoints <nbPoints>\n");
    exit(1); 
  }
  n = atoi(argv[1]); 
  printf("you entered n=%d\n", n);
  assert(n >0); 

  //allocate global arrays of n points 
  points = (point2D*)malloc(n*sizeof(point2D));
  assert(points); 
  //initialize_points_random();
  //initialize_points_star();
    //initialize_points_arrow();
    initialize_points_cubic();

  //print_points();

  Rtimer rt1; 
  rt_start(rt1); 
  hull = graham_scan(points,n); 
  rt_stop(rt1); 
  //print the hull 
  print_hull(); 
  //print the timing 
  char buf [1024]; 
  rt_sprint(buf,rt1);
  printf("finding convex hull with graham scan:  %s\n\n", buf);
  fflush(stdout); 

 
  

  /* initialize GLUT  */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);

  /* register callback functions */
  glutDisplayFunc(display); 
  glutKeyboardFunc(keypress);

  /* init GL */
  /* set background color black*/
  glClearColor(0, 0, 0, 0);   
  /* here we can enable depth testing and double buffering and so
     on */

  
  /* give control to event handler */
  glutMainLoop();
  return 0;
}


/* ****************************** */
/* draw the array of points stored in global variable points[] 
   each point is drawn as a small square 
  
*/
void draw_points(){

  const int R= 1;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //set color 
  glColor3fv(yellow);   
  
  assert(points);
  int i;
  for (i=0; i<n; i++) {
    //draw a small square centered at (points[i].x, points[i].y)
    glBegin(GL_POLYGON);
    glVertex2f(points[i].x -R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y-R);
    glVertex2f(points[i].x +R,points[i].y+R);
    glVertex2f(points[i].x -R,points[i].y+R);
    glEnd();
  }

}



/* ****************************** */
/* draw the list of points stored in global variable hull; the points
   are expected to be in order (ccw or cw) and consecutive points are
   connected by a line
*/
void draw_hull(){

  //set color 
  glColor3fv(red);   
  
  if (hull) {
    pointNode* prev = hull; 
    pointNode* crt = prev->next; 

    while (crt) {
      //draw a line from prev to crt 
      glBegin(GL_LINES);
      glVertex2f(prev->p.x, prev->p.y); 
      glVertex2f(crt->p.x, crt->p.y); 
      glEnd();
      prev=crt; 
      crt=crt->next; 
    }
    
    //draw a line from the last point to the first point 
    glBegin(GL_LINES);
    glVertex2f(prev->p.x, prev->p.y); 
    glVertex2f(hull->p.x, hull->p.y); 
    glEnd();
    prev=crt; 
    crt=crt->next; 
  
  }//if (hull)
}


/* ****************************** */
void display(void) {

  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); //clear the matrix


  /* the default GL window is [-1,1]x[-1,1] with the origin in the
     center the points are in the range (0,0) to (WINSIZE,WINSIZE), so
     they need to be mapped to [-1,1]x [-1,1] */
  glScalef(2.0/WINDOWSIZE, 2.0/WINDOWSIZE, 1.0);  
  glTranslatef(-WINDOWSIZE/2, -WINDOWSIZE/2, 0); 
  draw_points();
  draw_hull(); 

  /* execute the drawing commands */
  glFlush();
}



/* ****************************** */
void keypress(unsigned char key, int x, int y) {
  switch(key) {
  case 'q':
    exit(0);
    break;
  } 
}


/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
     
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   gluOrtho2D(0.0, (GLdouble) width, 0.0, (GLdouble) height); 
}




