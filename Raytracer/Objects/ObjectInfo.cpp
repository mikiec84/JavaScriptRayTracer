/**
 *  CPE 2010
 *  -------------------
 *  Program
 *
 *  Last Modified:
 *  @author Nick Feeney
 */

#include "ObjectInfo.h"
#include "LightSource.h"

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
void parseObjectPigment( FILE *file, ObjectInfo &info )
{
   char cur = '\0';

   while(cur != '<')
   {
      if( fscanf(file, "%c", &cur) == EOF)
      {
         printf("Error Parsing pigment\n");
         exit(1);
      }
   }
   float r = 0, g = 0, b = 0;
   if( fscanf(file, " %f , %f , %f ", &(r), &(g), &(b) ) == EOF )
   {
      printf("Error Parsing pigment\n");
      exit(1);
   }
   info.colorInfo.pigment.r = r;
   info.colorInfo.pigment.g = g;
   info.colorInfo.pigment.b = b;
   cur = ' ';
   while( isspace(cur) )
   {
      if( fscanf(file, "%c", &cur) == EOF)
      {
         printf("Error Parsing pigment\n");
         exit(1);
      }
   }
   if( cur == ',' )
   {
      if( fscanf(file, " %f ", &info.colorInfo.pigment_f ) == EOF )
      {
         printf("Error Parsing pigment\n");
         exit(1);
      }
   }
   else if ( cur == '>')
   {
   }
   else
   {
      printf("Error Parsing pigment\n");
      exit(1);
   }

   while(cur != '}')
   {
      if( fscanf(file, "%c", &cur) == EOF)
      {
         printf("Error Parsing pigment\n");
         exit(1);
      }
   }
   printf( " pigment: %f, %f, %f\n",  info.colorInfo.pigment.r, info.colorInfo.pigment.g, info.colorInfo.pigment.b );
}
void parseObjectFinish( FILE *file, ObjectInfo &info )
{
   char cur = ' ';
   while( cur != '{' )
   {
      if( fscanf(file, "%c", &cur) == EOF)
      {
         printf("Error parsing finish\n");
         exit(1);
      }
   }
   cur = ' ';

   while(cur != '}')
   {
      //Optional things
      while ( isspace( cur ) )
      {
         if( fscanf(file, "%c", &cur) == EOF)
         {
            printf("Error parsing finish\n");
            exit(1);
         }
      }
      //ambient
      if( cur == 'a' )
      {
         while(cur != 't')
         {
            if( fscanf(file, "%c", &cur) == EOF)
            {
               printf("Error parsing finish\n");
               exit(1);
            }
         }
         if( fscanf(file, " %f ", &(info.colorInfo.finish_ambient) ) == EOF )
         {
            printf("Error parsing finish\n");
            exit(1);
         }
         cur =  ' ';
      }
      //defuse
      else if( cur == 'd' )
      {
         //read in tell next number
         while(cur != 'e')
         {
            if( fscanf(file, "%c", &cur) == EOF)
            {
               printf("Error parsing finish\n");
               exit(1);
            }
         }
         if( fscanf(file, " %f ", &(info.colorInfo.finish_diffuse) ) == EOF )
         {
            printf("Error parsing finish\n");
            exit(1);
         }
         cur =  ' ';
      }
      //Specular
      else if( cur == 's' )
      {
         while( cur != 'r' )
         {
            if( fscanf(file, "%c", &cur) == EOF)
            {
               printf("Error parsing finish\n");
               exit(1);
            }
         }
         if( fscanf( file, " %f ", &(info.colorInfo.finish_specular)) == EOF)
         {
            printf("Error parsing finish\n");
            exit(1);
         }
         cur =  ' ';
      }
      //roughness or reflection, or refaction
      else if( cur == 'r' )
      {
         if( fscanf(file, "%c", &cur) == EOF)
         {
            printf("Error parsing finish\n");
            exit(1);
         }
         //roughness
         if( cur == 'o' )
         {
            while( cur != 's' )
            {
               if( fscanf(file, "%c", &cur) == EOF)
               {
                  printf("Error parsing finish\n");
                  exit(1);
               }
            }
            if( fscanf(file, "%c", &cur) == EOF)
            {
               printf("Error parsing finish\n");
               exit(1);
            }
            if( cur != 's' )
            {
               printf("Error parsing finish\n");
               exit(1);
            }
            if( fscanf( file, " %f ", &(info.colorInfo.finish_roughness)) == EOF)
            {
               printf("Error parsing finish\n");
               exit(1);
            }
            cur = ' ';
         }
         else if( cur == 'e' )
         {
            //reflection or refraction
            if( fscanf(file, "%c", &cur) == EOF)
            {
               printf("Error parsing finish\n");
               exit(1);
            }

            if( cur != 'f' )
            {
               printf("Error parsing finish\n");
               exit(1);
            }

            if( fscanf(file, "%c", &cur) == EOF)
            {
               printf("Error parsing finish\n");
               exit(1);
            }
            if(cur == 'l')
            {
               //reflection
               while( cur != 'n' )
               {
                  if( fscanf(file, "%c", &cur) == EOF)
                  {
                     printf("Error parsing finish\n");
                     exit(1);
                  }
               }
               if( fscanf( file, " %f ", &(info.colorInfo.finish_reflection)) == EOF)
               {
                  printf("Error parsing finish\n");
                  exit(1);
               }
            }
            else if( cur == 'r' )
            {
               //refraction
               while( cur != 'n' )
               {
                  if( fscanf(file, "%c", &cur) == EOF)
                  {
                     printf("Error parsing finish\n");
                     exit(1);
                  }
               }
               if( fscanf( file, " %f ", &(info.colorInfo.finish_refraction)) == EOF)
               {
                  printf("Error parsing finish\n");
                  exit(1);
               }
            }
            else
            {
               printf("Error parsing finish\n");
               exit(1);
            }
         }
         cur = ' ';
      }
      //ior See: "My Life for Aiur"
      else if( cur == 'i')
      {
         while( cur != 'r' )
         {
            if( fscanf(file, "%c", &cur) == EOF)
            {
               printf("Error parsing finish\n");
               exit(1);
            }
         }
         if( fscanf( file, " %f ", &(info.colorInfo.finish_ior)) == EOF)
         {
            printf("Error parsing finish\n");
            exit(1);
         }
         printf( "finish ior:%f\n", info.colorInfo.finish_ior);
         cur = ' ';
      }
      else if( cur == '}' )
      {
         //do nothing
      }
      else
      {
         printf("Error parsing finish\n");
         exit(1);
      }
   }
   printf( "Finish: %f, %f, %f, %f, %f, %f, %f\n", info.colorInfo.finish_ambient, info.colorInfo.finish_diffuse,
         info.colorInfo.finish_specular, info.colorInfo.finish_roughness,
         info.colorInfo.finish_reflection, info.colorInfo.finish_refraction, info.colorInfo.finish_ior);

}
void parseObjectTransforms( FILE *file, ObjectInfo &info )
{
   char cur = '\0';

   //read in until the end of the object
   while( cur != '}' )
   {
      if( fscanf( file, "%c", &cur ) == EOF)
      {
         printf("Error parsing Transforms\n");
         exit(1);
      }

      //Translate found
      if(cur == 't' || cur == 'T')
      {
         while( cur != '<' )
         {
            if( fscanf( file, "%c", &cur) == EOF )
            {
               printf("Error parsing Transforms\n");
               exit(1);
            }
         }
         vec3 translate;
         if( fscanf( file, " %f , %f , %f ", &(translate.x), &(translate.y), &(translate.z) ) == EOF )
         {
            printf("Error parsing Transforms\n");
            exit(1);
         }

         while( cur != '>' )
         {
            if( fscanf( file, "%c", &cur) == EOF )
            {
               printf("Error parsing Transforms\n");
               exit(1);
            }
         }
         printf(" translate: %f , %f , %f\n", translate.x, translate.y, translate.z );
         //add translate to transform matrix
      }
      //Scale found
      else if( cur == 's' || cur == 'S' )
      {
         while( cur != '<' )
         {
            if( fscanf( file, "%c", &cur) == EOF )
            {
               printf("Error parsing Transforms\n");
               exit(1);
            }
         }
         vec3 scale;
         if( fscanf( file, " %f , %f , %f ", &(scale.x), &(scale.y), &(scale.z) ) == EOF )
         {
            printf("Error parsing Transforms\n");
            exit(1);
         }

         while( cur != '>' )
         {
            if( fscanf( file, "%c", &cur) == EOF )
            {
               printf("Error parsing Transforms\n");
               exit(1);
            }
         }
         printf(" Scale: %f , %f , %f\n", scale.x, scale.y, scale.z );
      }
      //Rotate found
      else if( cur == 'r' || cur == 'R' )
      {
         while( cur != '<' )
         {
            if( fscanf( file, "%c", &cur) == EOF )
            {
               printf("Error parsing Transforms\n");
               exit(1);
            }
         }

         vec3 rotate;
         if( fscanf( file, " %f , %f , %f ", &(rotate.x), &(rotate.y), &(rotate.z) ) ==EOF )
         {
            printf("Error parsing Transforms\n");
            exit(1);
         }

         while( cur != '>' )
         {
            if( fscanf( file, "%c", &cur) == EOF )
            {
               printf("Error parsing Transforms\n");
               exit(1);
            }
         }
         printf(" Rotate: %f , %f , %f\n", rotate.x, rotate.y, rotate.z );
      }
   }
}
