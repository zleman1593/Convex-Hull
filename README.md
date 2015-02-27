# Convex-Hull
An implementation of the Graham Scan algorithm for finding the convex hull of  a set of points

Algorithm:
1. Used a stack (Stack class in C++) to store the points on the convex hull
2. The first point of reference is the one with the smallest y-coordinate 
3. Compare points to see if third point is to the left or right of the line segment formed by the first two points
4. If the point is to the left, add it to the stack; otherwise, pop it from the stack

Testing:
1. Imported different test cases to ViewPoints.cpp
2. To run, uncomment one of the test cases in lines 414-427
