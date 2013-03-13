#include "a4.hpp"

#include <algorithm>

#include "image.hpp"
#include "debug.hpp"
#include "primitive.hpp"
#include "material.hpp"

void a4_render(// What to render
               SceneNode* root,
               // Where to output the image
               const std::string& filename,
               // Image size
               int width, int height,
               // Viewing parameters
               const Point3D& eye, const Vector3D& view,
               const Vector3D& up, double fov,
               // Lighting parameters
               const Colour& ambient,
               const std::list<Light*>& lights
               )
{
  // Fill in raytracing code here.

  DEBUG_MSG( "Stub: a4_render(" << root << ",\n     "
            << filename << ", " << width << ", " << height << ",\n     "
            << eye << ", " << view << ", " << up << ", " << fov << ",\n     "
            << ambient << ",\n     {" );

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cerr << ", ";
    DEBUG_MSG(**I);
  }
  DEBUG_MSG( "});" << std::endl);
  
  // Camera Model from http://courses.csusm.edu/cs697exz/camera.html
 
  // convert fov to radians
  fov = fov * M_PI / 180.0;

  // Camera is pointing where?
  Vector3D cameraDir = view;
  cameraDir.normalize();

  Vector3D cameraUp = up;
  cameraUp.normalize();

  // Camera x axis
  Vector3D cameraU = cameraDir.cross(cameraUp);
  cameraU.normalize();
  // Camera y axis
  Vector3D cameraV = cameraU.cross(cameraDir);
  cameraV.normalize();

  Vector3D cameraDU = 2 * tan(fov/2.0) / width * cameraU;
  Vector3D cameraDV = 2 * tan(fov/2.0) / height * cameraV;



  // RHS coordinate system
  Image img(width, height, 3);

  // Loop through every pixel
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < height; x++) {

      // create a ray for this pixel

      Ray cameraRay;

      cameraRay.pos = eye;
      cameraRay.dir = cameraDir + -0.5 * ( 2 * y + 1 - height) * cameraDV
        + 0.5* (2*x + 1 - width) * cameraDU;
      cameraRay.dir.normalize();


     


      // find the closest intersection

      
      Intersection intersection;

      for (SceneNode::ChildList::const_iterator it = root->children().begin(), end = root->children().end(); it != end; ++it) {
        //DEBUG_MSG((*it)->name());
        GeometryNode *geoNode = dynamic_cast<GeometryNode*>(*it);
        assert(geoNode);
        
        
        Intersection i = geoNode->get_primitive()->intersect(cameraRay);
        
        if (i.hit)
        {
          assert(i.t > 0);
        }

        if (i.hit) {
          if (!intersection.hit || i.t < intersection.t) {
            intersection = i;
            intersection.mat = geoNode->get_material();
          }
        }

      }

      // given an intersection, compute if a light is visible
      // compute the color based on the light

      // Set the color

      // show background
      if (!intersection.hit) {

        // Red: increasing from top to bottom
        img(x, y, 0) = (double)y / height;
        // Green: increasing from left to right
        img(x, y, 1) = (double)x / width;
        // Blue: in lower-left and upper-right corners
        img(x, y, 2) = ((y < height/2 && x < height/2)
                        || (y >= height/2 && x >= height/2)) ? 1.0 : 0.0;
      }
      else {

        Vector3D color;
        //calculate lighting
        Material *m = intersection.mat;
        PhongMaterial *mat = dynamic_cast<PhongMaterial*>(m);
        assert(mat);

        for (std::list<Light*>::const_iterator it = lights.begin(), end = lights.end(); it != end; ++it) {
          Light * l = *it;

          //check if light is blocked by shadow

          // lambert only cares about the incoming direction of light against the normal
          // once it hits, the lighting is equal eveywehre
          // float lambert = (lightRay.dir * n) * coef;

          //ambient
          color[0] += mat->diffuse().R() * ambient.R();
          color[1] += mat->diffuse().G() * ambient.G();
          color[2] += mat->diffuse().B() * ambient.B();

          //diffuse
          Vector3D lightDir = l->position; - intersection.pos;
          lightDir.normalize();

          double lambert = (intersection.n.dot(lightDir)) * 1.0;
          color[0] += lambert * mat->diffuse().R() * l->colour.R();
          color[1] += lambert * mat->diffuse().G()* l->colour.G();
          color[2] += lambert * mat->diffuse().B()* l->colour.B();


          // phong
          double reflect = 2.0 * (lightDir.dot(intersection.n));
          Vector3D phongDir = lightDir - reflect * intersection.n;
          double phonCoeff = std::max(phongDir.dot(cameraRay.dir), 0.0);

          //DEBUG_MSG("phonCoeff " << phonCoeff << " ks " << mat->shininess());
          phonCoeff = pow(phonCoeff, mat->shininess()) * 1.0;
          //DEBUG_MSG("final phonCoeff " << phonCoeff );

          color[0] += phonCoeff * mat->specular().R() * l->colour.R();
          color[1] += phonCoeff * mat->specular().G()* l->colour.G();
          color[2] += phonCoeff * mat->specular().B()* l->colour.B();

          DEBUG_MSG("phonCoeff " << phonCoeff << " Lambert: " << lambert);
        }


        DEBUG_MSG("x" << x << " y:" << y << " " << color);

        img(x,y,0) = color[0]/2;
        img(x,y,1) = color[1]/2;
        img(x,y,2) = color[2]/2;

      }
    }
  }
  img.savePng(filename);
  
}
