/**
 *  CPE 2010
 *  -------------------
 *  Program
 *
 *  Last Modified:
 *  @author Nick Feeney
 */
#include "Master.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef char bool;
#define true 1
#define false 0

float planeHitTest( Plane plane,  Ray ray )
{
   vec3 direction = unit(ray.dir);
   vec3 position;
   vec3 normal = unit(plane.normal);
   direction.x = -direction.x;
   direction.y = -direction.y;
   direction.z = -direction.z;
   position.x = ray.pos.x;
   position.y = ray.pos.y;
   position.z = ray.pos.z;

   float vd = dot(normal, direction);
   if((distance < 0 && vd > -0.0001) || (distance > 0 && vd < 0.0001))
      return -1;
   float v0 = dot(newDirection(plane.point, position), plane.normal);
   float t = v0/vd;
   //make sure its pointing right directions
   if( t < 0 )
      t = -t;
   if( t < 0.001)
      return -1;
   return t;

}
Intersection planeIntersection(  Plane plane,  Ray ray, float t )
{
   Intersection ret;
   ret.hit = true;
   ret.viewVector.x = -ray.dir.x;
   ret.viewVector.y = -ray.dir.y;
   ret.viewVector.z = -ray.dir.z;

   ret.hitMark.x = ray.pos.x + ray.dir.x*t;
   ret.hitMark.y = ray.pos.y + ray.dir.y*t;
   ret.hitMark.z = ray.pos.z + ray.dir.z*t;
   ret.normal = plane.normal;
   ret.colorInfo = plane.info.colorInfo;
   return ret;
}

/**
 *  CPE 2010
 *  -------------------
 *  Program
 *
 *  Last Modified:
 *  @author Nick Feeney
 */

float sphereHitTest(  Sphere sphere,  Ray ray )
{
   vec3 direction = unit( ray.dir );
   float xc = sphere.pos.x;
   float yc = sphere.pos.y;
   float zc = sphere.pos.z;
   float x0 = ray.pos.x;
   float y0 = ray.pos.y;
   float z0 = ray.pos.z;
   float xd = direction.x;
   float yd = direction.y;
   float zd = direction.z;

   float A = xd*xd + yd*yd + zd*zd;
   float B = 2*(xd *(x0-xc) + yd*(y0-yc) + zd*(z0-zc));
   float C = (x0-xc)*(x0-xc) + (y0-yc)*(y0-yc) + (z0-zc)*(z0-zc) - sphere.radius*sphere.radius;
   float disc = B*B -4*A*C;
   if(disc < .0001)
      return -1;

   float t0 = (-B - sqrt(disc))/2;
   if ( t0 < 0.001)
   {
      t0 = (-B + sqrt(disc))/2;
   }
   if( t0 <= 0.001 )// && t0 >= -.00001 )
      return -1;
   return t0;
}
Intersection sphereIntersection(  Sphere sphere,  Ray ray, float t0 )
{
   vec3 direction = unit( ray.dir );
   Intersection ret;
   float xc = sphere.pos.x;
   float yc = sphere.pos.y;
   float zc = sphere.pos.z;
   float x0 = ray.pos.x;
   float y0 = ray.pos.y;
   float z0 = ray.pos.z;
   float xd = direction.x;
   float yd = direction.y;
   float zd = direction.z;
   ret.viewVector.x = -direction.x;
   ret.viewVector.y = -direction.y;
   ret.viewVector.z = -direction.z;
   ret.hitMark.x = x0 + xd*t0;
   ret.hitMark.y = y0 + yd*t0;
   ret.hitMark.z = z0 + zd*t0;

   vec3 objHit;
   objHit.x = ray.pos.x + direction.x* t0;
   objHit.y = ray.pos.y + direction.y* t0;
   objHit.z = ray.pos.z + direction.z* t0;

   ret.normal.x = (objHit.x - xc)/sphere.radius;
   ret.normal.y = (objHit.y - yc)/sphere.radius;
   ret.normal.z = (objHit.z - zc)/sphere.radius;
   ret.normal = unit(ret.normal);
   ret.colorInfo = sphere.info.colorInfo;

   ret.hit = true;
   return ret;
}


/**
 *  CPE 2010
 *  -------------------
 *  Program
 *
 *  Last Modified:
 *  @author Nick Feeney
 */

float triangleHitTest(  Triangle triangle,  Ray ray )
{
   vec3 direction = unit( ray.dir );
   vec3 position = ray.pos;
   direction.x = direction.x;
   direction.y = direction.y;
   direction.z = direction.z;

   float aa = triangle.a.x - triangle.b.x;
   float bb = triangle.a.y - triangle.b.y;
   float cc = triangle.a.z - triangle.b.z;
   float d = triangle.a.x - triangle.c.x;
   float e = triangle.a.y - triangle.c.y;
   float f = triangle.a.z - triangle.c.z;
   float g = direction.x;
   float h = direction.y;
   float i = direction.z;
   float j = triangle.a.x - position.x;
   float k = triangle.a.y - position.y;
   float l = triangle.a.z - position.z;

   float t = -1;
   float beta = 0;
   float gamma = 0;

   float ei_m_hf = e*i - h*f;
   float gf_m_di = g*f - d*i;
   float dh_m_eg = d*h - e*g;
   float ak_m_jb = aa*k - j*bb;
   float jc_m_al = j*cc - aa*l;
   float bl_m_kc = bb*l - k*cc;
   float M = aa*ei_m_hf + bb*gf_m_di + cc*dh_m_eg;
   if( M  < 0.0001 && M > -0.0001 )
   {
      return -1;
   }
   t = -(f*ak_m_jb + e*jc_m_al + d* bl_m_kc)/M;

   if(t < 0.001)
   {
      return -1;
   }

   gamma = (i*ak_m_jb + h*jc_m_al + g* bl_m_kc)/M;
   if(gamma < 0 || gamma > 1)
   {
      return -1;
   }

   beta = (j*ei_m_hf + k*gf_m_di + l*dh_m_eg)/M;
   if(beta < 0 || beta > (1 - gamma))
   {
      return -1;
   }

   return t;
}
Intersection triangleIntersection(  Triangle triangle,  Ray ray, float t )
{
   Intersection ret;

   ret.hit = true;
   vec3 direction = unit( ray.dir );
   ret.viewVector.x = -direction.x;
   ret.viewVector.y = -direction.y;
   ret.viewVector.z = -direction.z;
   ret.hitMark.x = ray.pos.x + direction.x*t;
   ret.hitMark.y = ray.pos.y + direction.y*t;
   ret.hitMark.z = ray.pos.z + direction.z*t;
   ret.normal = triangle.normal;
   ret.colorInfo = triangle.info.colorInfo;
   return ret;
}

/**
 *  CPE 2010
 *  -------------------
 *  Program
 *
 *  Last Modified:
 *  @author Nick Feeney
 */

#define PI 3.141592

ObjectInfo createObjectInfo()
{
   ObjectInfo obj;
   obj.colorInfo.pigment.r = 0;
   obj.colorInfo.pigment.g = 0;
   obj.colorInfo.pigment.b = 0;
   obj.colorInfo.pigment_f = 0;

   obj.colorInfo.finish_ambient = 0;
   obj.colorInfo.finish_diffuse = 0;
   obj.colorInfo.finish_specular = 0;
   obj.colorInfo.finish_roughness = 0;
   obj.colorInfo.finish_reflection = 0;
   obj.colorInfo.finish_refraction = 0;
   obj.colorInfo.finish_ior = 0;
   return obj;
}
/**
 *  CPE 2010
 *  -------------------
 *  Program
 *
 *  Last Modified:
 *  @author Nick Feeney
 */

#define PI 3.141592

int createInitRays( Ray **rays, int width, int height, Camera cam )
{
   vec3 right = unit(cam.right);
   vec3 up = unit(cam.up);
   float rightUnitX = right.x;
   float rightUnitY = right.y;
   float rightUnitZ = right.z;
   float upUnitX = up.x;
   float upUnitY = up.y;
   float upUnitZ = up.z;
   vec3 uv = unit(newDirection(cam.lookat, cam.pos));

   *rays = (Ray *) malloc( sizeof(Ray) *height*width );
   for( int i = 0; i < height; i++)
   {
      for( int j = 0; j < width ; j ++ )
      {
         float u = cam.l + (cam.r-cam.l)*( (float)j)/(float)width;
         float v = cam.b + (cam.t-cam.b)*( (float)i)/(float)height;
         float w = -1;
         int c = i*width + j;

         (*rays)[c].pos = cam.pos;
         (*rays)[c].dir.x = u * rightUnitX + v * upUnitX + -w * uv.x;
         (*rays)[c].dir.y = u * rightUnitY + v * upUnitY + -w * uv.y;
         (*rays)[c].dir.z = u * rightUnitZ + v * upUnitZ + -w * uv.z;
         (*rays)[c].i = i;
         (*rays)[c].j = j;
      }
   }
   return width * height;
}
void castRays( Scene scene, Ray *rays, int numRays, int width, int height, Color **buffer )
{
   for (int i = 0; i < numRays; i++)
       buffer[rays[i].i][rays[i].j] = raytrace( scene, rays[i] );
}
Color raytrace( Scene scene, Ray ray )
{
   Color color;
   color.r = 0;
   color.b = 0;
   color.g = 0;

   Intersection best;
   best.hit = false;

   float bestT = 10000;
   float t;
   for( int j = 0; j < scene.numSpheres; j++ )
   {
      t = sphereHitTest( scene.spheres[j], ray );
      if( t > 0 )
      {
         if( !best.hit || t < bestT )
         {
            best = sphereIntersection( scene.spheres[j], ray, t );
            bestT = t;
         }
      }
   }
   for( int j = 0; j < scene.numTriangles; j++ )
   {
      t = triangleHitTest( scene.triangles[j], ray );
      if( t > 0 )
      {
         if( !best.hit || t < bestT )
         {
            best = triangleIntersection( scene.triangles[j], ray, t );
            bestT = t;
         }
      }
   }
   for( int j = 0; j < scene.numPlanes; j++ )
   {
      t = planeHitTest( scene.planes[j], ray );
      if( t > 0 )
      {
         if( !best.hit || t < bestT )
         {
            best = planeIntersection( scene.planes[j], ray, t );
            bestT = t;
         }
      }
   }
   if( best.hit )
   {
      color = plus( color, directIllumination( best, scene ) );
      //printf("color: %f, %f, %f\n", color.r, color.g, color.b);
   }
   return limitColor( color );
}
Color limitColor(  Color in )
{
   Color ret;
   if( in.r > 1.0 )
      ret.r = 1.0;
   else if( in.r < 0.0 )
      ret.r = 0;
   else
      ret.r = in.r;

   if( in.g > 1.0 )
      ret.g = 1.0;
   else if( in.g < 0.0 )
      ret.g = 0;
   else 
      ret.g = in.g;

   if( in.b > 1.0 )
      ret.b = 1.0;
   else if( in.b < 0.0 )
      ret.b = 0;
   else
      ret.b = in.b;

   return ret;
}
Color plus(  Color first,  Color other )
{
   Color ret;
   ret.r = first.r + other.r;
   ret.g = first.g + other.g;
   ret.b = first.b + other.b;
   return limitColor( ret );
}

Color directIllumination( Intersection inter, Scene scene )
{
   Color ret;
   ret.r = 0;
   ret.b = 0;
   ret.g = 0;

   for( int i = 0; i < scene.numPointLights; i++ )
   {
      PointLight temp = scene.pointLights[i];
      vec3 lvec = unit(newDirection(temp.pos, inter.hitMark ));

      float nlDot = dot(lvec, inter.normal );
      bool inShadow = false;
      float lightDistance = distance( temp.pos, inter.hitMark );

      //contruct possible hits for shadow ray using bvh
      for( int j = 0; j < scene.numSpheres; j++ )
      {
         Ray shadowRay;
         shadowRay.pos = inter.hitMark;
         shadowRay.dir = lvec;
         float t = sphereHitTest(scene.spheres[j], shadowRay );
         if( t > 0 )
         {
            Intersection info = sphereIntersection( scene.spheres[j], shadowRay, t );
            if(info.hit)
            {
               if( distance( info.hitMark, inter.hitMark ) < lightDistance )
               {
                  inShadow = true;
                  break;
               }
            }
         }
      }

      ObjectInfo shadowInter;
      if( !inShadow )
      {
         vec3 r;
         r.x = -lvec.x + 2 * nlDot * inter.normal.x;
         r.y = -lvec.y + 2 * nlDot * inter.normal.y;
         r.z = -lvec.z + 2 * nlDot * inter.normal.z;
         r = unit(r);
         float rvDot = dot(r, unit(inter.viewVector));
         if(nlDot < 0)
            nlDot = 0;
         if(rvDot < 0)
            rvDot = 0;
         float powRV = pow( rvDot, 1.0/inter.colorInfo.finish_roughness );

         ret.r  =ret.r + temp.color.r * powRV*inter.colorInfo.finish_specular;
         ret.g = ret.g + temp.color.g* powRV*inter.colorInfo.finish_specular;
         ret.b = ret.b + temp.color.b * powRV*inter.colorInfo.finish_specular;
         ret.r+= inter.colorInfo.pigment.r * temp.color.r * nlDot*inter.colorInfo.finish_diffuse;
         ret.g += inter.colorInfo.pigment.g * temp.color.g * nlDot*inter.colorInfo.finish_diffuse;
         ret.b+= inter.colorInfo.pigment.b * temp.color.b * nlDot*inter.colorInfo.finish_diffuse;
         ret = limitColor( ret );
      }
   }
   //1.5 not 1 to increase the directlight which will be balanced in Util/tga.cpp during gamma correction
   float mod = 1;//.2 - finish_reflection - pigment_f*finish_refraction;
   ret.r = ret.r * mod;
   ret.g = ret.g * mod;
   ret.b = ret.b * mod;
   return ret;
}
/**
 *  CPE 2010
 *  -------------------
 *  Program
 *
 *  Last Modified:
 *  @author Nick Feeney
 */

float mag( vec3 in)
{
   return sqrt(in.x*in.x + in.y*in.y + in.z*in.z);
}
float dot( vec3 one,  vec3 two)
{
   return one.x*two.x + one.y*two.y + one.z*two.z;
}
vec3 cross( vec3 one, vec3 two)
{
   vec3 newVector;
   newVector.x = one.y*two.z - one.z*two.y;
   newVector.y = one.z*two.x - one.x*two.z;
   newVector.z = one.x*two.y - one.y*two.x;
   return newVector;
}
float theta( vec3 one,  vec3 two)
{
   return acosf( dot(one, two)/(mag(one) * mag(two))) * 180.0 / 3.14159;
}
float distance( vec3 one,  vec3 two )
{
   return sqrt((one.x-two.x)*(one.x-two.x) + (one.y-two.y)*(one.y-two.y) + (one.z-two.z)*(one.z-two.z));
}
vec3 newDirection( vec3 to,  vec3 from )
{
   vec3 newVec;
   newVec.x = to.x - from.x;
   newVec.y = to.y - from.y;
   newVec.z = to.z - from.z;
   return newVec;
}
vec3 unit( vec3 in)
{
   float temp;
   vec3 newVector;
   newVector.x = 0;
   newVector.y = 0;
   newVector.z = 0;
   temp = mag(in);

   if(temp > 0)
   {
      newVector.x = in.x/temp;
      newVector.y = in.y/temp;
      newVector.z = in.z/temp;
   }
   return newVector;
}

