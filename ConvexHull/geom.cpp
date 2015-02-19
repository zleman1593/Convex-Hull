#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stack>

static const int LEFT = -1;
static const int COLINEAR = 0;
static const int RIGHT = 1;

point2D p_0;
std::stack<point2D> s;


/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
 is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2D a, point2D b, point2D c) {
    
    return 1;
}



/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2D p, point2D q, point2D r) {
    
    return 1;
}



/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left (point2D a, point2D b, point2D c) {
    
    return 1;
}

/* returns -1 if c is left of ab, 0 if colinear, and 1 if c is right of ab. */
int cmpDirection(point2D a, point2D b, point2D c) {
    //Calculate 2D cross-product
    int direction = (b.y - a.y) * (c.x - a.x) - (b.x - a.x) * (c.y - a.y);
    
    if (direction == 0) {
        
        return 0;// colinear triplet
    }
    else if (direction > 0){
        return 1;//Right
    }else{
        return -1;//left
        
    }
    
}

/* Sort points in increasing order of polar angle with respect to Po
 */
int cmpfunc(const void * a, const void * b) {
    /*Type cast*/
    point2D* p = (point2D*) a;
    point2D* q = (point2D*) b;
    
    
    
    return cmpDirection(p_0, *p, *q);
}

/* compute the convex hull of the points in p; the points on the CH are returned as a list
 */
pointNode* graham_scan(point2D* p, int n) {
    // Starting point is the one with lowest y coordinate.
    p_0 = p[0];
    int lowest_y = INT32_MAX;
    int index = 0;
    for (int i = 0; i < n; i++) {
        point2D current = p[i];
        // If two points have the same y, take the one with smaller x.
        if (current.y < lowest_y || (current.y == lowest_y && current.x < p_0.x)) {
            p_0 = current;
            index = i;
        }
    }
    // Move p_0 to front of the array.
    point2D temp = p[0];
    p[0] = p_0;
    p[index] = temp;
    
    // Sort the other n - 1 points by angle.
    qsort(&p[1], n - 1, sizeof(point2D), cmpfunc);
    
    // Initialize
    s.push(p_0);
    s.push(p[1]);
    
    // Iterate over points and push on stack as necessary.
    int i = 2;
    while (i < n) {
        point2D first = p[i - 2];
        point2D second = p[i - 1];
        if (cmpDirection(p_0, first, second) == LEFT) {
            s.push(p[i]);
            i++;
        } else {
            s.pop();
        }
    }
    
    return NULL; 
}

