////create RAYS needs a vec3 struct with  x,y,z as floats
Ray *createIRays(Scene scene, int width, int height )
{
   Ray *rays = (Ray *) malloc( sizeof(Ray) * width *height );
   for( int i = 0; i < height; i++ )
   {
      for( int j = 0; j < width; j++ )
      {
         float xp = j*1.0/width - .5;
         float yp = i*1.0/height - .5;
         rays[i*width + j] = getRay( xp, yp, scene );
      }
   }
   return rays;
}
Ray getRay( float x, float y, Scene scene )
{
   Ray ray;
   ray.pos.x = scene.camera.eye.x;
   ray.pos.y = scene.camera.eye.y;
   ray.pos.z = scene.camera.eye.z;

   vec3 up;
   up.x = 0;
   up.y = 1;
   up.z = 0;

   vec3 cr = newDir( lookat, ray.pos );
   cr = unit(cross(cr, up));
   cr = scalarMult( cr, x );
   
   up = scalarMult( up, y );

   vec3 lookat;
   lookat = scene.camera.lookat.x + cr.x + up.x;
   lookat = scene.camera.lookat.y + cr.y + up.y;
   lookat = scene.camera.lookat.z + cr.z + up.z;

   ray.dir = unit(newDir( lookat, ray.pos ));
   return ray;
}
vec3 newDir( vec3 one, vec3 two )
{
   vec3 n;
   n.x = one.x - two.x;
   n.y = one.y - two.y;
   n.z = one.z - two.z;
   return n;
}
vec3 cross( vec3 one, vec two )
{
   //TODO:cross product
   return one;
}
vec3 unit( vec3 in )
{
   //TODO: normalize
   return in;
}
////////////////////getPixelColor aka main cast ray
Color getPixelColor( Ray ray, Scene scene )
{
   RayIntersection info = testIntersection( ray, scene );
   if( info.isHit )
   {
      return rayTrace( info, ray, scene );
   }
   Color color;
   color.r = 0;
   color.g = 0;
   color.b = 0;
   return color;
}

RayIntersection testIntersection( Ray ray, Scene scene )
{
   int hits = 0;
   RayIntersection best;
   best.isHit = 0;
   best.distance = 2000;

   for( int i = 0; i < scene.numSpheres; i++ )
   {
      RayIntersection info = sphereIntersection( scene.spheres[i], ray );
      if( info.isHit ** info.distance >= 0 ** info.distance < best.distance )
      {
         best = info;
         hits++;
      }
   }
   return best;
}

