#include "a4.hpp"

#include <algorithm>
#include <pthread.h>

#include "image.hpp"
#include "debug.hpp"
#include "primitive.hpp"
#include "material.hpp"
#define NUM_THREADS	8

char *messages[NUM_THREADS];

struct thread_data
{
	int	thread_id;
	int  sum;
	char *message;
	int rowModulo;
	int rowDivisor;	// exclusive

	SceneNode* root;
	// Where to output the image
	//const std::string& filename,
	Image *img;
	// Image size
	int width;
	int height;
	// Viewing parameters
	Point3D eye; Vector3D view; Vector3D up; double fov;
	// Lighting parameters
	Colour ambient;
	std::list<Light*> lights;

};

struct thread_data thread_data_array[NUM_THREADS];


void a4_render_helper(
		int rowModulo, int rowDivisor,
		// What to render
		SceneNode* root,
		// Where to output the image
		Image &img,
		// Image size
		int width, int height,
		// Viewing parameters
		const Point3D& eye, const Vector3D& view, const Vector3D& up, double fov,
		// Lighting parameters
		Colour ambient, std::list<Light*> lights)
{
	// Fill in raytracing code here.

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

	// Loop through every pixel
	for (int y = 0; y < height; y++)
	{
		if (y % rowDivisor != rowModulo) continue;

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

}

void *PrintHello(void *threadarg)
{
	int taskid, sum;
	char *hello_msg;
	struct thread_data *my_data;

	my_data = (struct thread_data *) threadarg;
	taskid = my_data->thread_id;
	sum = my_data->sum;
	hello_msg = my_data->message;
	printf("Thread %d: %s  Sum=%d\n", taskid, hello_msg, sum);

	a4_render_helper(my_data->rowModulo, my_data->rowDivisor,
			my_data->root,
			*my_data->img,
			my_data->width, my_data->height,
			my_data->eye, my_data->view, my_data->up,
			my_data->fov, my_data->ambient, my_data->lights);

	pthread_exit(NULL);
}



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

	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	int *taskids[NUM_THREADS];
	int rc, t, sum;
	void *status;

	sum = 0;

	// setup thread attrs
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (t = 0; t < NUM_THREADS; t++)
	{
		sum = sum + t;
		thread_data_array[t].thread_id = t;
		thread_data_array[t].sum = sum;
		thread_data_array[t].message = messages[t];


		thread_data_array[t].rowModulo = t;
		thread_data_array[t].rowDivisor = NUM_THREADS;



		thread_data_array[t].root = root;
		thread_data_array[t].img = &img;
		thread_data_array[t].width = width;
		thread_data_array[t].height = height;
		thread_data_array[t].eye = eye;
		thread_data_array[t].view = view;
		thread_data_array[t].up = up;
		thread_data_array[t].fov = fov;
		thread_data_array[t].ambient = ambient;
		thread_data_array[t].lights = lights;



		printf("Creating thread %d\n", t);
		rc = pthread_create(&threads[t], &attr, PrintHello, (void *) &thread_data_array[t]);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	// wait for threads to finish
	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(t=0; t<NUM_THREADS; t++) {
		rc = pthread_join(threads[t], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
	}

	printf("Main: program completed. Saving File.\n");


	img.savePng(filename);

	DEBUG_MSG("Saved to " << filename);

}

