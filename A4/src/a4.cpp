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

  std::cerr << "Stub: a4_render(" << root << ",\n     "
            << filename << ", " << width << ", " << height << ",\n     "
            << eye << ", " << view << ", " << up << ", " << fov << ",\n     "
            << ambient << ",\n     {";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    if (I != lights.begin()) std::cerr << ", ";
    std::cerr << **I;
  }
  std::cerr << "});" << std::endl;
  
  // For now, just make a sample image.


  // RHS coordinate system
  Image img(width, height, 3);

  // Loop through every pixel
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < height; x++) {

      // create a ray for this pixel
      Vector3D rayOrigin(x - width/2.f,height/2.f - y , 800.f);
      Vector3D rayDirection(0.f, 0.f, -1.f);

      // find the closest intersection

      double bestIntersection = -1;
      GeometryNode *node = NULL;
      Vector3D bestNormal;
      Vector3D intersection;
      for (SceneNode::ChildList::const_iterator it = root->children().begin(), end = root->children().end(); it != end; ++it) {
        //DEBUG_MSG((*it)->name());
        GeometryNode *geoNode = dynamic_cast<GeometryNode*>(*it);
        assert(geoNode);
        NonhierSphere * sphere = dynamic_cast<NonhierSphere*>(geoNode->get_primitive());
        assert(sphere);
        Vector3D normal;
        Vector3D t_intersection;
        double t = sphere->intersect(rayOrigin, rayDirection, t_intersection, normal);

        if (t > 0) {
          if (bestIntersection < 0 || t < bestIntersection) {
            bestIntersection = t;
            node = geoNode;
            bestNormal = normal;
            intersection = t_intersection;
          }
        }

      }

      // given an intersection, compute if a light is visible
      // compute the color based on the light


      // Set the color

      // show background
      if (bestIntersection < 0) {

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
        Material *m = node->get_material();
        PhongMaterial *mat = dynamic_cast<PhongMaterial*>(m);
        assert(mat);

        for (std::list<Light*>::const_iterator it = lights.begin(), end = lights.end(); it != end; ++it) {
          Light * l = *it;

          //check if light is blocked by shadow

          // lambert only cares about the incoming direction of light against the normal
          // once it hits, the lighting is equal eveywehre
          // float lambert = (lightRay.dir * n) * coef;

          Vector3D lightDir = l->position; - intersection;
          lightDir.normalize();

          double lambert = (bestNormal.dot(lightDir)) * 0.5;
          color[0] += lambert * mat->diffuse().R() * l->colour.R();
          color[1] += lambert * mat->diffuse().G()* l->colour.G();
          color[2] += lambert * mat->diffuse().B()* l->colour.B();


          // phong
          double reflect = 2.0 * (lightDir.dot(bestNormal));
          Vector3D phongDir = lightDir - reflect * bestNormal;
          double phonCoeff = std::max(phongDir.dot(rayDirection), 0.0);

          DEBUG_MSG("phonCoeff " << phonCoeff << " ks " << mat->shininess());
          phonCoeff = pow(phonCoeff, mat->shininess()) * 1.0;
          DEBUG_MSG("final phonCoeff " << phonCoeff );

          color[0] += phonCoeff * mat->specular().R() * l->colour.R();
          color[1] += phonCoeff * mat->specular().G()* l->colour.G();
          color[2] += phonCoeff * mat->specular().B()* l->colour.B();


        }


        DEBUG_MSG("x" << x << " y:" << y << " " << color);

        img(x,y,0) = color[0];
        img(x,y,1) = color[1];
        img(x,y,2) = color[2];

      }
    }
  }
  img.savePng(filename);
  
}
