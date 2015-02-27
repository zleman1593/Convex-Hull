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
    
    if (direction == COLINEAR) {
        return COLINEAR;// colinear triplet
    } else if (direction > 0){
        return RIGHT;//Right
    } else {
        return LEFT;//left
    }
}

int distance(point2D a, point2D b)
{
    return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}


/* Sort points in increasing order of polar angle with respect to Po
 */
int cmpfunc(const void * a, const void * b) {
    /*Type cast*/
    point2D* p = (point2D*) a;
    point2D* q = (point2D*) b;
    
    int direction = cmpDirection(p_0, *p, *q);
    if (direction == COLINEAR) {
        //If points have same polar angle then add the closest one first
        if (distance(p_0, *p) <= distance(p_0, *q)) {
            return LEFT;
        }
        return RIGHT;
    }
    return direction;
}

// Return second element from top of stack.
point2D secondFromTopInStack(std::stack<point2D> &stack){
    point2D topPoint = stack.top();
    stack.pop();
    point2D secondPointFromTop = stack.top();
    stack.push(topPoint);
    return secondPointFromTop;
}


pointNode* createListFromFinalStack(){
    pointNode *node;
    pointNode *head = NULL;
    while (!s.empty()) {
        node = (pointNode *)malloc(sizeof(pointNode));
        point2D top = s.top();
        printf("X COORD: %d\n", top.x);
        if (top.x != 0 || top.y != 0) {
            node->p = top;
            node->next = head;
            head = node;
        }
        s.pop();
    }
    return head;
}


/* Compute the convex hull of the points in p; the points on the CH are returned as a list
 */
pointNode* graham_scan(point2D* p, int n) {
    
    // Starting point is the one with lowest y coordinate.
    p_0 = p[0];
    int lowest_y = INT32_MAX;
    int index = 0;
    for (int i = 0; i < n; i++) {
        point2D current = p[i];
        // If two points have the same y, take the one with bigger x.
        if (current.y < lowest_y || (current.y == lowest_y && current.x > p_0.x)) {
            p_0 = current;
            lowest_y = current.y;
            index = i;
        }
    }
    
    // Move p_0 to front of the array.
    point2D temp = p[0];
    p[0] = p_0;
    p[index] = temp;

    // Sort the other n - 1 points by polar angle/distance wrt Po.
    qsort(&p[1], n - 1, sizeof(point2D), cmpfunc);
    
    // Initialize
    s.push(p[0]);
    s.push(p[1]);
    
    // Iterate over points and push on stack as necessary.
    int i = 2;
    while (i < n) {
        // Stack has 0 or 1 element
        if (s.size() == 1) {
            s.push(p[i]);
            i++;
        } else if (s.size() == 0 && i < n - 2) {
            s.push(p[i]);
            s.push(p[i + 1]);
            i += 2;
        }

        // Get points from stack.
        point2D first = secondFromTopInStack(s);
        point2D second = s.top();
        point2D third = p[i];
        
        if (cmpDirection(first, second, third) == LEFT) {
            s.push(p[i]);
            i++;
        } else {
            s.pop();
        }
    }
    return createListFromFinalStack();
}



