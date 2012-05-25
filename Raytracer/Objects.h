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

typedef int t_shape;

typedef struct vec3 {
   float x;
   float y; 
   float z;
} vec3;

typedef struct Sphere { 
   vec3 pos;
   float radius;
} Sphere;

typedef struct Plane {
   vec3 normal;
   float distance;
} Plane;

typedef struct Ray {
   vec3 pos;
   vec3 dir;
} Ray;

typedef struct Color {
   float r;
   float g;
   float b;
} Color;

typedef struct Light {
   vec3 position;
   Color color;
   float intensity;
} Light;

typedef struct IntersectionInfo {
   int isHit;
   int hitCount;
   t_shape shape;
   vec3 position;
   vec3 normal;
   Color color;
   float distance;
} IntersectionInfo;

//Vector Helper Functions
void vCopy(vec3 * d, vec3 *s);
void vNormalize(vec3 *v);
float vMagnitude(vec3 *v);
void vCross(vec3 *v0, vec3 *v1, vec3 *r);
float vDot(vec3 *v0, vec3 *v1);
void vAdd(vec3 *v0, vec3 *v1, vec3 *r);
void vSubtract(vec3 *v0, vec3 *v1, vec3 *r);
void vMultVector(vec3 *v0, vec3 *v1, vec3 *r);
void vMultScalar(vec3 *v, float s, vec3 *r);

//Color Helper Functions
void cCopy(Color *d, Color *s);
void cAdd(Color *c0, Color *c1, Color *r);
void cAddScalar(Color *c, float s);
void cSubtract(Color *c0, Color *c1, Color *r);
void cMultiply(Color *c0, Color *c1, Color *r);
void cMultiplyScalar(Color *c, float s);
void cDivideFactor(Color *c, float f);
void cLimit(Color *c);
void cDistance(Color *c0, Color *c1);
void cBlend(Color *c0, Color *c1, float w, Color *r);

//Light Helper Functions
float getLightIntensity(Light * light, float distance);

#endif
