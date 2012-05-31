var DIM = 400;

function vec3(x, y, z) {
	this.x = x;
	this.y = y;
	this.z = z;
}

function Camera(pos, up, right, lookAt, l, r, b, t) {
	this.pos = pos;
	this.up = up;
	this.right = right;
	this.lookAt = lookAt;
	this.l = l;
	this.r = r;
	this.b = b;
	this.t = t;
}

function Color(r, g, b, a) {
	this.r = r;
	this.g = g;
	this.b = b;
	this.a = a;
}

function PointLight(color, pos) {
	this.color = color; // Color
	this.pos = pos; // vec3
}

function ColorInfo(
		finish_ambient,
		finish_diffuse,
		finish_specular,
		finish_roughness,
		finish_reflection,
		finish_refraction,
		finish_ior,
		pigment_f,
		pigment) {
	this.finish_ambient = finish_ambient;
	this.finish_diffuse = finish_diffuse;
	this.finish_specular = finish_specular;
	this.finish_roughness = finish_roughness;
	this.finish_reflection = finish_reflection;
	this.finish_refraction = finish_refraction;
	this.finish_ior = finish_ior;
	this.pigment_f = pigment_f;
	this.pigment = pigment; // Color
}

function ObjectInfo(colorInfo) {
	this.colorInfo = colorInfo; // ColorInfo
}

function Plane(distancee, normal, point, info) {
	this.distancee = distancee;
	this.normal = normal; // vec3
	this.point = point; // vec3
	this.info = info; // ObjectInfo
}

function Sphere(radius, pos, info) {
	this.radius = radius;
	this.pos = pos; // vec3
	this.info = info; // ObjectInfo
}

function Triangle(distancee, a, b, c, normal, info) {
	this.distancee = distancee;
	this.a = a; // vec3
	this.b = b; // vec3
	this.c = c; // vec3
	this.normal = normal; // vec3
	this.info = info; // ObjectInfo
}

function Intersection(hitMark, normal, viewVector, hit, colorInfo) {
	this.hitMark = hitMark; // vec3
	this.normal = normal; // vec3
	this.viewVector = viewVector; // vec3
	this.hit = hit; // var
	this.colorInfo = colorInfo; // ColorInfo
}

function Ray(pos, dir, i, j) {
	this.pos = pos; // vec3
	this.dir = dir; // vec3
	this.i = i; // int
	this.j = j; // int
}

function Scene(spheres, planes, triangles, pointLights, camera) {
	this.spheres = spheres; // Sphere[]
	this.planes = planes; // Plane[]
	this.triangles = triangles; // Triangle[]
	this.pointLights = pointLights; // PointLight[]
	this.camera = camera; // Camera
}

  

function mag(inVec) {
	return Math.sqrt(inVec.x * inVec.x + inVec.y * inVec.y + inVec.z * inVec.z);
}
function dott(one, two) {
	return one.x * two.x + one.y * two.y + one.z * two.z;
}
function crosss(one, two) {
	return new vec3(
			one.y * two.z - one.z * two.y,
			one.z * two.x - one.x * two.z,
			one.x * two.y - one.y * two.x);
}
function theta(one, two) {
	return Math.acos(dott(one, two) / (mag(one) * mag(two))) * 180.0 / 3.14159;
}
function distancee(one, two) {
	return Math.sqrt((one.x-two.x)*(one.x-two.x) + (one.y-two.y)*(one.y-two.y) + (one.z-two.z)*(one.z-two.z));
}
function newDirection(to, from) {
	return new vec3(to.x - from.x, to.y - from.y, to.z - from.z);
}
function unit(inVec) {
	var temp = mag(inVec);
	if (temp > 0)
		return new vec3(inVec.x / temp, inVec.y / temp, inVec.z / temp);
	return new vec3(0, 0, 0);
}

function planeHitTest(plane, ray) {
	var direction = unit(ray.dir);
	direction = new vec3(-direction.x, -direction.y, -direction.z);
	var position = new vec3(ray.pos.x, ray.pos.y, ray.pos.z);
	var normal = unit(plane.normal);
	
	var vd = dott(normal, direction);
	if ((plane.distancee < 0 && vd > -0.0001) || (plane.distancee > 0 && vd < 0.0001))
		return -1;
	var v0 = dott(newDirection(plane.point, position), plane.normal);
	var t = v0 / vd;
	if (t < 0)
		t = -t;
	if (t < 0.001)
		return -1;
	return t;
}

function planeIntersection(plane, ray, t) {
	return new Intersection(
			new vec3(
					ray.pos.x + ray.dir.x * t,
					ray.pos.y + ray.dir.y * t,
					ray.pos.z + ray.dir.z * t),
			plane.normal,
			new vec3(-ray.dir.x, -ray.dir.y, -ray.dir.z),
			1,
			plane.info.colorInfo);
}

function sphereHitTest(sphere, ray) {
	var direction = unit(ray.dir);
	var xc = sphere.pos.x;
	var yc = sphere.pos.y;
	var zc = sphere.pos.z;
	var x0 = ray.pos.x;
	var y0 = ray.pos.y;
	var z0 = ray.pos.z;
	var xd = direction.x;
	var yd = direction.y;
	var zd = direction.z;
	
	var A = xd * xd + yd * yd + zd * zd;
	var B = 2 * (xd * (x0 - xc) + yd * (y0 - yc) + zd * (z0 - zc));
	var C = (x0-xc)*(x0-xc) + (y0-yc)*(y0-yc) + (z0-zc)*(z0-zc) - sphere.radius*sphere.radius;
	var disc = B * B - 4 * A * C;
	if (disc < .0001)
		return -1;
		
	var t0 = (-B - Math.sqrt(disc)) / 2;
	if (t0 < 0.001)
		t0 = (-B + Math.sqrt(disc)) / 2;
	if (t0 <= 0.001)
		return -1;
	return t0;
}

function sphereIntersection(sphere, ray, t0) {
	var direction = unit(ray.dir);
	var xc = sphere.pos.x;
	var yc = sphere.pos.y;
	var zc = sphere.pos.z;
	var x0 = ray.pos.x;
	var y0 = ray.pos.y;
	var z0 = ray.pos.z;
	var xd = direction.x;
	var yd = direction.y;
	var zd = direction.z;
	
   var objHit = new vec3();
   objHit.x = ray.pos.x + direction.x* t0;
   objHit.y = ray.pos.y + direction.y* t0;
   objHit.z = ray.pos.z + direction.z* t0;

	return new Intersection(
			new vec3(
					x0 + xd * t0,
					y0 + yd * t0,
					z0 + zd * t0),
			unit(new vec3(
					(objHit.x - xc) / sphere.radius,
					(objHit.y - yc) / sphere.radius,
					(objHit.z - zc) / sphere.radius)),
			new vec3(-direction.x, -direction.y, -direction.z),
			1,
			sphere.info.colorInfo);
}

function triangleHitTest(triangle, ray ) {
	var direction = new vec3(direction.x, direction.y, direction.z);
	var position = ray.pos;
	
	var aa = triangle.a.x - triangle.b.x;
	var bb = triangle.a.y - triangle.b.y;
	var cc = triangle.a.z - triangle.b.z;
	var d = triangle.a.x - triangle.c.x;
	var e = triangle.a.y - triangle.c.y;
	var f = triangle.a.z - triangle.c.z;
	var g = direction.x;
	var h = direction.y;
	var i = direction.z;
	var j = triangle.a.x - position.x;
	var k = triangle.a.y - position.y;
	var l = triangle.a.z - position.z;
	
	var t = -1;
	var beta = 0;
	var gamma = 0;
	
	var ei_m_hf = e*i - h*f;
	var gf_m_di = g*f - d*i;
	var dh_m_eg = d*h - e*g;
	var ak_m_jb = aa*k - j*bb;
	var jc_m_al = j*cc - aa*l;
	var bl_m_kc = bb*l - k*cc;
	var M = aa*ei_m_hf + bb*gf_m_di + cc*dh_m_eg;
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

function triangleIntersection(triangle, ray, t) {
	return new Intersection(
			new vec3(
					ray.pos.x + direction.x * t,
					ray.pos.y + direction.y * t,
					ray.pos.z + direction.z * t),
			triangle.normal,
			new vec3(-direction.x, -direction.y, -direction.z),
			1,
			triangle.info.colorInfo);
}

var PI = 3.141592;

function createObjectInfo() {
	return new ObjectInfo(
			new ColorInfo(
					0, 0, 0, 0,
					0, 0,
					0,
					0,
					new Color(0, 0, 0)));
}

function createInitRays(rays, width, height, cam) {
   var right = unit(cam.right);
   var up = unit(cam.up);
   var rightUnitX = right.x;
   var rightUnitY = right.y;
   var rightUnitZ = right.z;
   var upUnitX = up.x;
   var upUnitY = up.y;
   var upUnitZ = up.z;
   var uv = unit(newDirection(cam.lookAt, cam.pos));

   //*rays = (Ray *) malloc( sizeof(Ray) *height*width );
   for( var i = 0; i < height; i++)
   {
      for( var j = 0; j < width ; j ++ )
      {
         var u = cam.l + (cam.r-cam.l)*j/width;
         var v = cam.b + (cam.t-cam.b)*i/height;
         var w = -1;
         var c = i*width + j;

					rays[c] = new Ray();
         rays[c].pos = cam.pos;
         rays[c].dir = new vec3();
         rays[c].dir.x = u * rightUnitX + v * upUnitX + -w * uv.x;
         rays[c].dir.y = u * rightUnitY + v * upUnitY + -w * uv.y;
         rays[c].dir.z = u * rightUnitZ + v * upUnitZ + -w * uv.z;
         rays[c].i = i;
         rays[c].j = j;
      }
   }
}
function clCastRays(rays, buffer)
{
	var scene = getScene();
	
	for (var i = 0; i < DIM * DIM; i++) {
		buffer[i] = raytrace(scene, rays[i]);
	}
}

function raytrace(scene, ray )
{
	var color = new Color(0, 0, 0, 1);
	
	var best = new Intersection();
	best.hit = 0;
   
   var bestT = 10000;
   var t = 0;
   
   for( var j = 0; j < scene.spheres.length; j++ )
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
   for( var j = 0; j < scene.triangles.length; j++ )
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
   for( var j = 0; j < scene.planes.length; j++ )
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
function limitColor( inc )
{
   var ret = new Color();
   
   if( inc.r > 1.0 )
      ret.r = 1.0;
   else if( inc.r < 0.0 )
      ret.r = 0;
   else
      ret.r = inc.r;

   if( inc.g > 1.0 )
      ret.g = 1.0;
   else if( inc.g < 0.0 )
      ret.g = 0;
   else 
      ret.g = inc.g;

   if( inc.b > 1.0 )
      ret.b = 1.0;
   else if( inc.b < 0.0 )
      ret.b = 0;
   else
      ret.b = inc.b;

   return ret;
}
function plus( first, other )
{
   var ret = new Color(
   		first.r + other.r,
			first.g + other.g,
			first.b + other.b);
   return limitColor( ret );
}

function directIllumination( inter, scene )
{
   ret = new Color(0, 0, 0, 1);

   for( var i = 0; i < scene.pointLights.length; i++ )
   {
      var temp = scene.pointLights[i];
      var lvec = unit(newDirection(temp.pos, inter.hitMark ));

      var nldott = dott(lvec, inter.normal );
      var inShadow = 0;
      var lightdistancee = distancee( temp.pos, inter.hitMark );

      //contruct possible hits for shadow ray using bvh
      for( var j = 0; j < scene.spheres.length; j++ )
      {
         var shadowRay = new Ray();
         shadowRay.pos = inter.hitMark;
         shadowRay.dir = lvec;
         var t = sphereHitTest(scene.spheres[j], shadowRay );
         if( t > 0 )
         {
            var info = sphereIntersection( scene.spheres[j], shadowRay, t );
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

      var shadowInter = new ObjectInfo();
      if( !inShadow )
      {
         var r = new vec3();
         r.x = -lvec.x + 2 * nldott * inter.normal.x;
         r.y = -lvec.y + 2 * nldott * inter.normal.y;
         r.z = -lvec.z + 2 * nldott * inter.normal.z;
         r = unit(r);
         var rvdott = dott(r, unit(inter.viewVector));
         if(nldott < 0)
            nldott = 0;
         if(rvdott < 0)
            rvdott = 0;
         var powRV = Math.pow( rvdott, 1.0/inter.colorInfo.finish_roughness );

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
   var mod = 1;//.2 - finish_reflection - pigment_f*finish_refraction;
   ret.r = ret.r * mod;
   ret.g = ret.g * mod;
   ret.b = ret.b * mod;
   return ret;
}

function clRayTrace( rays, image) {
   scene = getScene();
   createInitRays( rays, DIM, DIM, scene.camera );
}
  
function getScene() {
	var right = new vec3(1.33, 0, 0);
	var up = new vec3(0, 1, 0);
	
	return new Scene(
			[
					new Sphere(
							2,
							new vec3(0, 0, 0),
							new ObjectInfo(
									new ColorInfo(
											.2, .4, 0, 0,
											0, 0,
											0,
											0,
											new Color(1.0, 0, 1.0, 1.0))))
			],
			[],
			[],
			[
					new PointLight(new Color(1.0, 1.0, 1.0, 1.0), new vec3(-100, 100, 100)),
			],
			new Camera(
					new vec3(0, 0, 14), // pos
					up, // up
					right, // right
					new vec3(0, 0, 0), // look at
					-mag(right) / 2,
					mag(right) / 2,
					-mag(up) / 2,
					mag(up) / 2));
}