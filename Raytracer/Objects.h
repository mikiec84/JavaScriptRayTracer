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
   float distance;
}
typedef struct Ray {
   vec3 pos;
   vec3 dir;
}
typedef struct Color {
   float r;
   float g;
   float b;
}

vec3 unit( vec3 in );
float dot( vec3 first, vec3 second );
#endif
