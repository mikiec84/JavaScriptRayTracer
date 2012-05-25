/**
 *  CPE 530
 *  -------------------
 *  Color Helper Functions 
 *
 *  Last Modified: 
 *  @author Tyler Casella
 */

#include "Objects.h"

void cCopy(Color *d, Color *s) {
	d->r = s->r;
	d->g = s->g;
	d->b = s->b;
}

void cAdd(Color *c0, Color *c1, Color *r) {
	r->r = c0->r + c1->r;
	r->g = c0->g + c1->g;
	r->b = c0->b + c1->b;
}

void cAddScalar(Color *c, float s) {
	c->r += s;
	c->g += s;
	c->b += s;
}

void cSubtract(Color *c0, Color *c1, Color *r) {
	r->r = c0->r - c1->r;
	r->g = c0->g - c1->g;
	r->b = c0->b - c1->b;
}

void cMultiply(Color *c0, Color *c1, Color *r) {
	r->r = c0->r * c1->r;
	r->g = c0->g * c1->g;
	r->b = c0->b * c1->b;
}

void cMultiplyScalar(Color *c, float s) {
	c->r *= s;
	c->g *= s;
	c->b *= s;
}

void cDivideFactor(Color *c, float f) {
	c->r /= f;
	c->g /= f;
	c->b /= f;
}

void cLimit(Color *c) {
	c->r = (c->r > 0.f) ? ((c->r > 1.f) ? 1.f : c->r) : 0.f;
	c->g = (c->g > 0.f) ? ((c->g > 1.f) ? 1.f : c->g) : 0.f;
	c->b = (c->b > 0.f) ? ((c->b > 1.f) ? 1.f : c->b) : 0.f;
}

void cDistance(Color *c0, Color *c1) {
	return abs(c0->r - c1->r) + abs(c0->g - c1->g) + abs(c0->b - c1->b);
}

void cBlend(Color *c0, Color *c1, float w, Color *r) {
	//Create temporary arrays of the two colors
	float _c0[3];
	_c0[0] = c0[0]; _c0[1]] = c0[1]]; _c0[2]] = c0[2]];
	float _c1[3];
	_c1[0] = c1[0]; _c1[1]] = c1[1]]; _c1[2]] = c1[2]];
	
	//Multiply them by the factor (and inverse factor)
	cMultiplyScalar(_c0, 1.f - w);
	cMultiplyScalar(_c1, w);
	//Add them together and place into result color
	cAdd(&_c0, &_c1, r);
}