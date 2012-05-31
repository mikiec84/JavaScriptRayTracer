#define DIM 400

typedef struct vec3{
   float x;
   float y;
   float z;
} vec3;

typedef struct Camera {
   vec3 pos;
   vec3 up;
   vec3 right;
   vec3 lookat;
   float l,r,b,t;
} Camera;

#ifndef COLOR_H
#define COLOR_H
typedef struct Color {
   float r;
   float g;
   float b;
   float a;
} Color;
#endif

typedef struct PointLight {
   Color color;
   vec3 pos;
} PointLight;

typedef struct ColorInfo {
   float finish_ambient;
   float finish_diffuse;
   float finish_specular;
   float finish_roughness;
   float finish_reflection;
   float finish_refraction;
   float finish_ior;
   float pigment_f;
   Color pigment;
} ColorInfo;

typedef struct ObjectInfo {
   ColorInfo colorInfo;
} ObjectInfo;

typedef struct Plane {
   float distancee;
   vec3 normal;
   vec3 point;
   ObjectInfo info;
} Plane;

typedef struct Sphere {
   float radius;
   vec3 pos;
   ObjectInfo info;
} Sphere;

typedef struct Triangle {
   float distancee;
   vec3 a;
   vec3 b;
   vec3 c;
   vec3 normal;
   ObjectInfo info;
} Triangle;

typedef struct Intersection {
   vec3 hitMark;
   vec3 normal;
   vec3 viewVector;
   float hit;
   ColorInfo colorInfo;
} Intersection;

typedef struct Ray {
   vec3 pos;
   vec3 dir;
   int i, j;
} Ray;

typedef struct Scene {
   int numSpheres;
   int numTriangles;
   int numPlanes;
   int numPointLights;
   
   struct Sphere spheres[1];
   struct Plane planes[1];
   struct Triangle triangles[1];
   struct PointLight pointLights[1];
   struct Camera camera;

} Scene;

ObjectInfo createObjectInfo();

float planeHitTest(  Plane sphere,  Ray ray );
Intersection planeIntersection(  Plane sphere,  Ray ray, float t );

float sphereHitTest(  Sphere sphere,  Ray ray ); 
Intersection sphereIntersection(  Sphere sphere,  Ray ray, float t );

float triangleHitTest(  Triangle triangle,  Ray ray ); 
Intersection triangleIntersection(  Triangle triangle,  Ray ray, float t );

Color limitColor(  Color in );
Color plus(  Color first,  Color other );

Color directIllumination( Intersection inter, __global Scene *scene );

void createInitRays( __global struct Ray *rays, int width, int height, struct Camera cam );
//void castRays( __global struct Ray *rays, __global uchar4 *buffer);
Color raytrace(__global Scene* scene, Ray ray );

void getScene(__global Scene* scene);

float mag( vec3 in);
float dott( vec3 one,  vec3 two);
vec3 crosss( vec3 one,  vec3 two);
float theta( vec3 one,  vec3 two);
float distancee( vec3 one,  vec3 two );
vec3 newDirection( vec3 to,  vec3 from );
vec3 unit( vec3 in);
    
/**
 *  CPE 2010
 *  -------------------
 *  Program
 *
 *  Last Modified:
 *  @author Nick Feeney
 */



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

   float vd = dott(normal, direction);
   if((plane.distancee < 0 && vd > -0.0001) || (plane.distancee > 0 && vd < 0.0001))
      return -1;
   float v0 = dott(newDirection(plane.point, position), plane.normal);
   float t = v0/vd;
   //make sure its pointing right directions
   if( t < 0 )
      t = -t;
   if( t < 0.001)
      return -1;
   return t;

   return 0;
}
Intersection planeIntersection(  Plane plane,  Ray ray, float t )
{
   Intersection ret;
   ret.hit = 1;
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

   ret.hit = 1;
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

   ret.hit = 1;
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

void createInitRays( __global Ray *rays, int width, int height, Camera cam )
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

   //*rays = (Ray *) malloc( sizeof(Ray) *height*width );
   for( int i = 0; i < height; i++)
   {
      for( int j = 0; j < width ; j ++ )
      {
         float u = cam.l + (cam.r-cam.l)*( (float)j)/(float)width;
         float v = cam.b + (cam.t-cam.b)*( (float)i)/(float)height;
         float w = -1;
         int c = i*width + j;

         rays[c].pos = cam.pos;
         (rays)[c].dir.x = u * rightUnitX + v * upUnitX + -w * uv.x;
         (rays)[c].dir.y = u * rightUnitY + v * upUnitY + -w * uv.y;
         (rays)[c].dir.z = u * rightUnitZ + v * upUnitZ + -w * uv.z;
         (rays)[c].i = i;
         (rays)[c].j = j;
      }
   }
}
 __kernel void clCastRays(__global Scene* scene, __global Ray* rays, __global uchar4* buffer)
{
   int col = get_global_id(0);
   int row = get_global_id(1);
   int i = col*DIM + row;

   if (col >= DIM || row >= DIM) return;   
   
   Color color = raytrace( scene, rays[i] );
   buffer[rays[i].i*DIM + rays[i].j] = (uchar4)(255*color.r,255*color.g,255*color.b, 255);
	
	
}
Color raytrace(__global Scene* scene, Ray ray )
{
   Color color;
   color.r = 0; // todo: change back to 0, we want black background
   color.b = 0;
   color.g = 0;
   
   Intersection best;
   best.hit = 0;
   

   float bestT = 10000;
   float t = 0;
   
   for( int j = 0; j < scene->numSpheres; j++ )
   {
      t = sphereHitTest( scene->spheres[j], ray );
	  
      if( t > 0 )
      {
         if( !best.hit || t < bestT )
         {
            best = sphereIntersection( scene->spheres[j], ray, t );
            bestT = t;
         }
      }
	  
   }
   for( int j = 0; j < scene->numTriangles; j++ )
   {
      t = triangleHitTest( scene->triangles[j], ray );
      if( t > 0 )
      {
         if( !best.hit || t < bestT )
         {
            best = triangleIntersection( scene->triangles[j], ray, t );
            bestT = t;
         }
      }
   }
   for( int j = 0; j < scene->numPlanes; j++ )
   {
      t = planeHitTest( scene->planes[j], ray );
      if( t > 0 )
      {
         if( !best.hit || t < bestT )
         {
            best = planeIntersection( scene->planes[j], ray, t );
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

Color directIllumination( Intersection inter, __global Scene* scene )
{
   Color ret;
   ret.r = 0;
   ret.b = 0;
   ret.g = 0;

   for( int i = 0; i < scene->numPointLights; i++ )
   {
      PointLight temp = scene->pointLights[i];
      vec3 lvec = unit(newDirection(temp.pos, inter.hitMark ));

      float nldott = dott(lvec, inter.normal );
      int inShadow = 0;
      float lightdistancee = distancee( temp.pos, inter.hitMark );

      //contruct possible hits for shadow ray using bvh
      for( int j = 0; j < scene->numSpheres; j++ )
      {
         Ray shadowRay;
         shadowRay.pos = inter.hitMark;
         shadowRay.dir = lvec;
         float t = sphereHitTest(scene->spheres[j], shadowRay );
         if( t > 0 )
         {
            Intersection info = sphereIntersection( scene->spheres[j], shadowRay, t );
            if(info.hit)
            {
               if( distancee( info.hitMark, inter.hitMark ) < lightdistancee )
               {
                  inShadow = 1;
                  break;
               }
            }
         }
      }

      ObjectInfo shadowInter;
      if( !inShadow )
      {
         vec3 r;
         r.x = -lvec.x + 2 * nldott * inter.normal.x;
         r.y = -lvec.y + 2 * nldott * inter.normal.y;
         r.z = -lvec.z + 2 * nldott * inter.normal.z;
         r = unit(r);
         float rvdott = dott(r, unit(inter.viewVector));
         if(nldott < 0)
            nldott = 0;
         if(rvdott < 0)
            rvdott = 0;
         float powRV = pow( rvdott, 1.0/inter.colorInfo.finish_roughness );

         ret.r  =ret.r + temp.color.r * powRV*inter.colorInfo.finish_specular;
         ret.g = ret.g + temp.color.g* powRV*inter.colorInfo.finish_specular;
         ret.b = ret.b + temp.color.b * powRV*inter.colorInfo.finish_specular;
         ret.r+= inter.colorInfo.pigment.r * temp.color.r * nldott*inter.colorInfo.finish_diffuse;
         ret.g += inter.colorInfo.pigment.g * temp.color.g * nldott*inter.colorInfo.finish_diffuse;
         ret.b+= inter.colorInfo.pigment.b * temp.color.b * nldott*inter.colorInfo.finish_diffuse;
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
float dott( vec3 one,  vec3 two)
{
   return one.x*two.x + one.y*two.y + one.z*two.z;
}
vec3 crosss( vec3 one, vec3 two)
{
   vec3 newVector;
   newVector.x = one.y*two.z - one.z*two.y;
   newVector.y = one.z*two.x - one.x*two.z;
   newVector.z = one.x*two.y - one.y*two.x;
   return newVector;
}
float theta( vec3 one,  vec3 two)
{
   return acos( dott(one, two)/(mag(one) * mag(two))) * 180.0 / 3.14159;
}
float distancee( vec3 one,  vec3 two )
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

/*Web CL Stuff */
  __kernel void clRayTrace( __global Scene* scene,
							__global Ray* rays,
							__global uchar4* image)
  {
   getScene(scene);
   createInitRays( rays, DIM, DIM, scene->camera );
  // castRays(rays, image);
  }
  

  
  void  getScene(__global Scene* scene) {
   scene->numSpheres =2;
   scene->numPlanes = 0;
   scene->numTriangles = 0;
   scene->numPointLights = 1;

   scene->spheres[0].pos.x = -2;
   scene->spheres[0].pos.y = 0;
   scene->spheres[0].pos.z = 0;
   scene->spheres[0].info = createObjectInfo();
   scene->spheres[0].info.colorInfo.pigment.r = 1.0;
   scene->spheres[0].info.colorInfo.pigment.g = 0;
   scene->spheres[0].info.colorInfo.pigment.b = 1.0;
   scene->spheres[0].info.colorInfo.finish_ambient = 0.2;
   scene->spheres[0].info.colorInfo.finish_diffuse = 0.4;
   scene->spheres[0].radius = 2;
   
   scene->spheres[1].pos.x = 6;
   scene->spheres[1].pos.y = 0;
   scene->spheres[1].pos.z = 0;
   scene->spheres[1].info = createObjectInfo();
   scene->spheres[1].info.colorInfo.pigment.r = 1.0;
   scene->spheres[1].info.colorInfo.pigment.g = 0;
   scene->spheres[1].info.colorInfo.pigment.b = 1.0;
   scene->spheres[1].info.colorInfo.finish_ambient = 0.2;
   scene->spheres[1].info.colorInfo.finish_diffuse = 0.4;
   scene->spheres[1].radius = 2;

   scene->camera.up.x = 0;
   scene->camera.up.y = 1;
   scene->camera.up.z = 0;
   scene->camera.right.x = 1.33333;
   scene->camera.right.y = 0;
   scene->camera.right.z = 0;
   scene->camera.lookat.x = 0;
   scene->camera.lookat.y = 0;
   scene->camera.lookat.z = 0;
   scene->camera.pos.x = 0;
   scene->camera.pos.y = 0;
   scene->camera.pos.z = 14;
   scene->camera.l = -mag(scene->camera.right)/2;
   scene->camera.r = mag(scene->camera.right)/2;
   scene->camera.t = mag(scene->camera.up)/2;
   scene->camera.b = -mag(scene->camera.up)/2;
   
   scene->pointLights[0].pos.x = -100;
   scene->pointLights[0].pos.y = 100;
   scene->pointLights[0].pos.z = 100;
   scene->pointLights[0].color.r = 1.0;
   scene->pointLights[0].color.g = 1.0;
   scene->pointLights[0].color.b = 1.0;
  }
  