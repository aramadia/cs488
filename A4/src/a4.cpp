#include "a4.hpp"

#include <algorithm>

#include "image.hpp"
#include "debug.hpp"
#include "primitive.hpp"
#include "material.hpp"

void a4_render(
// What to render
		SceneNode* root,
		// Where to output the image
		const std::string& filename,
		// Image size
		int width, int height,
		// Viewing parameters
		const Point3D& eye, const Vector3D& view, const Vector3D& up, double fov,
		// Lighting parameters
		const Colour& ambient, const std::list<Light*>& lights)
{
	// Fill in raytracing code here.

	DEBUG_MSG(
			"Stub: a4_render(" << root << ",\n     " << filename << ", " << width << ", " << height << ",\n     " << eye << ", " << view << ", " << up << ", " << fov << ",\n     " << ambient << ",\n     {");

	for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I)
	{
		if (I != lights.begin())
			std::cerr << ", ";
		DEBUG_MSG(**I);
	}
	DEBUG_MSG("});" << std::endl);

	// Camera Model from http://courses.csusm.edu/cs697exz/camera.html

	// convert fov to radians
//	double fovy = fov * M_PI / 180.0;
//
//	const double aspect = (double)width/(double)height;
//
//	// Camera is pointing where?
//	Vector3D cameraDir = view;
//	cameraDir.normalize();
//
//	Vector3D cameraUp = up;
//	cameraUp.normalize();
//
//	// Camera x axis
//	Vector3D cameraU = cameraDir.cross(cameraUp);
//	cameraU.normalize();
//	// Camera y axis
//	Vector3D cameraV = cameraU.cross(cameraDir);
//	cameraV.normalize();
//
//	Vector3D cameraDU = 2.0 * aspect * tan(fovy / 2.0) / (double)width * cameraU;
//	Vector3D cameraDV = 2.0 * tan(fovy / 2.0) / (double)height * cameraV;

	double fov_radius = fov*M_PI/360.0;
		//2. get side vector
		Vector3D side_vector = view.cross(up);
		Vector3D m_view = view;
		Vector3D m_up = up;
		//normalize view, side, up
		side_vector.normalize();
		m_view.normalize();
		m_up.normalize();

	// RHS coordinate system
	Image img(width, height, 3);

	// Loop through every pixel
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < height; x++)
		{

			// create a ray for this pixel

			Ray cameraRay;

			cameraRay.pos = eye;
			cameraRay.dir = m_view + (x / (double(width)) * 2 -1 )* tan(fov_radius) * (double(width)/double(height)) * side_vector
					 + ((y) / (double(height)) * 2 - 1) * tan(fov_radius) * -(m_up) ;
			cameraRay.dir.normalize();

			// find the closest intersection

			Intersection intersection;
			intersection = root->intersect(cameraRay);



			// given an intersection, compute if a light is visible
			// compute the color based on the light

			// Set the color

			// show background
			if (!intersection.hit)
			{

				// take the angles of the vector
				double latitude = sin(cameraRay.dir[1]);	// -pi/2 to pi/2
				// scale to 0 to 1
				latitude = (latitude/M_PI) + 0.5;
				img(x,y,0) = latitude;
				img(x,y,1) = 0.1 + latitude * 0.3;
				img(x,y,2) = latitude * 0.2;

			}
			else
			{

				Vector3D color;
				//calculate lighting
				Material *m = intersection.mat;
				PhongMaterial *mat = dynamic_cast<PhongMaterial*>(m);
				assert(mat);

				for (std::list<Light*>::const_iterator lightIt = lights.begin(), end = lights.end(); lightIt != end; ++lightIt)
				{
					Light * l = *lightIt;

					//diffuse
					Vector3D lightDir = l->position - intersection.pos;
					lightDir.normalize();

					//check if light is blocked by shadow
					Ray lightRay;
					lightRay.dir = intersection.pos - l->position;
					lightRay.dir.normalize();
					lightRay.pos = l->position;
					bool inShadow = false;
					const double distanceToLight = (intersection.pos - l->position).length();


					Intersection lightIntersection = root->intersect(lightRay);
					if (lightIntersection.hit)
					{
						// check if the intersection happens before intersection.pos plus a buffer
						if ((lightIntersection.pos - l->position).length() < distanceToLight - 0.001)
						{
							inShadow = true;
						}
					}



					// Don't process thist light
					if (inShadow)
					{
						// TODO
						continue;
					}

					// lambert only cares about the incoming direction of light against the normal
					// once it hits, the lighting is equal eveywehre
					// float lambert = (lightRay.dir * n) * coef;

					double lambert = (intersection.n.dot(lightDir)) * 1.0;
					color[0] += lambert * mat->diffuse().R() * l->colour.R();
					color[1] += lambert * mat->diffuse().G() * l->colour.G();
					color[2] += lambert * mat->diffuse().B() * l->colour.B();

					// phong
					double reflect = 2.0 * (lightDir.dot(intersection.n));
					Vector3D phongDir = lightDir - reflect * intersection.n;
					double phonCoeff = std::max(phongDir.dot(cameraRay.dir), 0.0);

					//DEBUG_MSG("phonCoeff " << phonCoeff << " ks " << mat->shininess());
					phonCoeff = pow(phonCoeff, mat->shininess()) * 1.0;
					//DEBUG_MSG("final phonCoeff " << phonCoeff );

					color[0] += phonCoeff * mat->specular().R() * l->colour.R();
					color[1] += phonCoeff * mat->specular().G() * l->colour.G();
					color[2] += phonCoeff * mat->specular().B() * l->colour.B();

					DEBUG_MSG("phonCoeff " << phonCoeff << " Lambert: " << lambert);
				}

				//ambient
				color[0] += mat->diffuse().R() * ambient.R();
				color[1] += mat->diffuse().G() * ambient.G();
				color[2] += mat->diffuse().B() * ambient.B();

				DEBUG_MSG("x" << x << " y:" << y << " " << color);

				img(x, y, 0) = color[0] / 2;
				img(x, y, 1) = color[1] / 2;
				img(x, y, 2) = color[2] / 2;

			}
		}
	}
	img.savePng(filename);

	DEBUG_MSG("Saved to " << filename);

}
