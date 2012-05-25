/**
 *  CPE 530
 *  -------------------
 *  Vector Helper Functions 
 *
 *  Last Modified: 
 *  @author Tyler Casella
 */

#include "Objects.h"

void vCopy(vec3 * d, vec3 *s) {
	d->x = s->x;
	d->y = s->y;
	d->z = s->z;
}

void vNormalize(vec3 *v) {
	float mag = vMagnitude(v);
	v->x /= mag;
	v->y /= mag;
	v->z /= mag;
}

float vMagnitude(vec3 *v) {
	return sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

void vCross(vec3 *v0, vec3 *v1, vec3 *r) {
	r->x = -v0->z * v1->y + v0->y * v1->z;
	r->y = v0->z * v1->x - v0->x * v1->z;
	r->z = -v0->y * v1->x + v0->x * v1->y;
}

float vDot(vec3 *v0, vec3 *v1) {
	return (v0->x * v1->x + v0->y * v1->y + v0->z * v1->z);
}

void vAdd(vec3 *v0, vec3 *v1, vec3 *r) {
	r->x = v0->x + v1->x;
	r->y = v0->y + v1->y;
	r->z = v0->z + v1->z;
}

void vSubtract(vec3 *v0, vec3 *v1, vec3 *r) {
	r->x = v0->x - v1->x;
	r->y = v0->y - v1->y;
	r->z = v0->z - v1->z;
}

void vMultVector(vec3 *v0, vec3 *v1, vec3 *r) {
	r->x = v0->x * v1->x;
	r->y = v0->y * v1->y;
	r->z = v0->z * v1->z;
}

void vMultScalar(vec3 *v, float s, vec3 *r) {
	r->x = v->x * s;
	r->y = v->y * s;
	r->z = v->z * s;
}