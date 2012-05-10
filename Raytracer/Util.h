/**
 *  CPE 2010
 *  -------------------
 *  Program 
 *
 *  Last Modified: 
 *  @author Nick Feeney
 */

#ifndef UTIL_H
#define UTIL_H
#include "Objects.h"

Color intersection( Sphere obj, Ray ray, int t ); 
Color intersection( Plane obj, Ray ray, int t ); 
int hitTest( Sphere obj, Ray ray );
int hitTest( Plane obj, Ray ray );

#endif
