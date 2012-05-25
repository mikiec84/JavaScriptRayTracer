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

typedef struct PointLight {
   Color color;
   vec3 pos;
} PointLight;

typedef struct Color {
   float r;
   float g;
   float b;
} Color;

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
   float distance;
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
   float distance;
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
   struct Sphere *spheres;
   struct Plane *planes;
   struct Triangle *triangles;
   struct PointLight *pointLights;
   struct Camera camera;

   int numSpheres;
   int numTriangles;
   int numPlanes;
   int numPointLights;
} Scene;

ObjectInfo createObjectInfo();

float planeHitTest( const Plane &sphere, const Ray &ray );
Intersection planeIntersection( const Plane &sphere, const Ray &ray, float t );

float sphereHitTest( const Sphere &sphere, const Ray &ray ); 
Intersection sphereIntersection( const Sphere &sphere, const Ray &ray, float t );

float triangleHitTest( const Triangle &triangle, const Ray &ray ); 
Intersection triangleIntersection( const Triangle &triangle, const Ray &ray, float t );

Color limitColor( const Color &in );
Color plus( const Color &first, const Color &other );

Color directIllumination( Intersection inter, Scene scene );

int createInitRays( struct Ray **rays, int width, int height, struct Camera cam );
void castRays( struct Scene scene, struct Ray *rays, int numRays, int width, int height, Color **buffer);
Color raytrace( struct Scene scene, Ray ray );

float mag(const vec3 &in);
float dot(const vec3 &one, const vec3 &two);
vec3 cross(const vec3 &one, const vec3 &two);
float theta(const vec3 &one, const vec3 &two);
float distance(const vec3 &one, const vec3 &two );
vec3 newDirection(const vec3 &to, const vec3 &from );
vec3 unit(const vec3 &in);