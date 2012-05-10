/**
 *  CPE 2010
 *  -------------------
 *  Program 
 *
 *  Last Modified: 
 *  @author Nick Feeney
 */

#ifndef OBJECTS_H
#define OBJECTS_H
typedef struct vec3 {
   float x;
   float y; 
   float z;
}
typedef struct Sphere { 
   vec3 pos;
   float radius;
} Sphere;
typedef struct Plane {
   vec3 normal;
   vec3 pos;
}
typedef struct Ray {
   vec3 pos;
   vec3 dir;
}
#endif
