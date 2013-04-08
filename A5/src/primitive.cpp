#include "primitive.hpp"
#include <vector>
#include <math.h>
#include "debug.hpp"
#include "mesh.hpp"
Primitive::~Primitive()
{
}

Sphere::Sphere()
{
	m_sphere = new NonhierSphere(Point3D(0.0, 0.0, 0.0), 1.0);
}

Sphere::~Sphere()
{
	delete m_sphere;
}

Intersection Sphere::intersect(Ray ray) {
	return m_sphere->intersect(ray);
}

Cube::Cube()
{
	m_cube = new NonhierBox(Point3D(0,0,0), 1.0);

}

Cube::~Cube()
{
	delete m_cube;
}

Intersection Cube::intersect(Ray ray) {
	return m_cube->intersect(ray);
}

NonhierSphere::~NonhierSphere()
{
}


Intersection NonhierSphere::intersect(Ray ray) {


	Intersection intersection;

	Vector3D dist = Vector3D(m_pos) - ray.pos; 
	double B = ray.dir.dot( dist);
	double D = B*B - dist.dot( dist) + m_radius * m_radius; 

	if (D < 0.0)  {
	    return intersection;	// no intersection
	}
	double t0 = B - sqrt(D); 
	double t1 = B + sqrt(D);
	double t = -1;  

	if ((t0 > 0.1f) ) 
	{
	    
	    t = t0;

	} 
	if ((t1 > 0.1f) && (t1 < t)) 
	{
	    t = t1;
	}


	if (t > 0) {
		intersection.t = t;
		intersection.hit = true;
		intersection.pos = ray.pos + ray.dir * t;
		intersection.n = intersection.pos - m_pos;
		intersection.n.normalize();
		//intersection.mat = 
	}

	return intersection;
}

NonhierBox::~NonhierBox()
{
	delete m_mesh;
}
NonhierBox::NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
{
	// create mesh
	//	 Mesh(const std::vector<Point3D>& verts,
	//	       const std::vector< std::vector<int> >& faces);

		std::vector<Point3D> verts;
		verts.push_back(Point3D(m_pos[0], 			m_pos[1], m_pos[2]));
		verts.push_back(Point3D(m_pos[0] + m_size, 	m_pos[1], m_pos[2]));
		verts.push_back(Point3D(m_pos[0] + m_size, 	m_pos[1], m_pos[2] + m_size));
		verts.push_back(Point3D(m_pos[0], 			m_pos[1], m_pos[2] + m_size));


		verts.push_back(Point3D(m_pos[0], 			m_pos[1] + m_size, m_pos[2]));
		verts.push_back(Point3D(m_pos[0] + m_size, 	m_pos[1] + m_size, m_pos[2]));
		verts.push_back(Point3D(m_pos[0] + m_size, 	m_pos[1] + m_size, m_pos[2] + m_size));
		verts.push_back(Point3D(m_pos[0], 			m_pos[1] + m_size, m_pos[2] + m_size));

		std::vector<std::vector<int> > faces;
		int faceArray[6][4] =
		{
				//{0,1,2,3},
				{3,2,1,0},
				//{7,3,2,6},
				{6,2,3,7},
				//{6,2,1,5},
				{5,1,2,6},

				//{5,1,0,4},
				{4,0,1,5},

				//{4,0,3,7},
				{7,3,0,4},

				//{4,7,6,5}
				{5,6,7,4}

		};

		for (int f = 0 ;f < 6; f++) {
			std::vector<int> polygon;

			for (int v = 0; v < 4; v++) {
				polygon.push_back(faceArray[f][v]);
			}
			faces.push_back(polygon);
		}






		// create the 8 verticies

		// create the 6 faces

		m_mesh = new Mesh(verts, faces);

}
Intersection NonhierBox::intersect(Ray ray) {
	Intersection intersection;


	// intersect like mesh

	return m_mesh->intersect(ray);
}



NonhierFractal::~NonhierFractal()
{

}
NonhierFractal::NonhierFractal(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
{


}
/*
 * uniform int Iterations;  slider[0,9,100]

// Mandelbulb exponent (8 is standard)
uniform float Power; slider[-10,8,10]

// Bailout radius
uniform float Bailout; slider[0,5,30]

uniform vec3 RotVector; slider[(0,0,0),(1,1,1),(1,1,1)]

uniform float RotAngle; slider[0.00,0,180]

uniform bool Julia; checkbox[false]
uniform vec3 JuliaC; slider[(-2,-2,-2),(0,0,0),(2,2,2)]
 */

const float Power = 8.0;

void powN2(Vector3D &z, double zr0) {
	float zo0 = asin( z[2]/zr0 );
	float zi0 = atan2( z[1],z[0] );
	float zr = pow( zr0, Power-1.0 );
	float zo = zo0 * Power;
	float zi = zi0 * Power;
	zr *= zr0;
	z  = zr*Vector3D( cos(zo)*cos(zi), cos(zo)*sin(zi), sin(zo) );
}

bool inside(Vector3D pos) {
	const float Bailout = 10;
	const int Iterations = 50;
	const bool Julia = false;
	const Vector3D JuliaC(0,0,0);
	Vector3D z=pos;
	double r;
	int i=0;
	r=z.length();

	while(r<Bailout && (i<Iterations)) {
		powN2(z,r);
		z = z + (Julia ? JuliaC : pos);
		r=z.length();
		i++;
	}
	return (r<Bailout);

}
const int Samples = 2000;
double closest = 1.0;
/*
 *
Near = 0.7368
Far = 2.45904
 */
const double Far = 5.0;
const double Near = 0.0;


Intersection fractalIntersect(Ray ray)
{
	Intersection intersection;

	intersection.hit = false;


	// evaluate without a distance estimator
	for (int i=0; i<Samples; i++) {
		float randf = (float)rand()/(float)RAND_MAX;
		double t = Near+ (Far - Near) * randf* closest;


		Vector3D point = ray.pos + t* ray.dir;
		//STATUS_MSG("Point: " << point);
		if (inside(point)) {
			//STATUS_MSG("HIT INSIDE POINT");
			closest = t;
			intersection.t = t;

			intersection.pos = Point3D(point[0], point[1], point[2]);
			intersection.hit = true;
			intersection.n = Vector3D(0,0,1);
		}
	}

	return intersection;
}


Intersection NonhierFractal::intersect(Ray ray) {
	Intersection intersection;

	intersection.hit = false;




	intersection = fractalIntersect(ray);

	if (intersection.hit) {
		// calculate normal vector

		// intersect two points around it, then do a cross product
		Ray xRay = ray;
		xRay.pos[0] += 0.00001;

		Intersection intersectX = fractalIntersect(xRay);


		Ray yRay = ray;
		yRay.pos[1] += 0.00001;

		Intersection intersectY = fractalIntersect(yRay);

		if (intersectX.hit && intersectY.hit) {
			Vector3D dx = intersectX.pos - intersection.pos;
			Vector3D dy = intersectY.pos -intersection.pos;
			intersection.n = cross(dy, dx);
			intersection.n.normalize();
			//STATUS_MSG("good");

		}
		else
		{
			//STATUS_MSG("edge?");
		}
	}

	// Hit position in world space.
//	Vector3D worldPos = Eye + (Near+tex.w*(Far-Near)) * rayDir;

//	vec3 n = normalize(cross(dFdx(worldPos), dFdy(worldPos)));
//
//
//	intersection.t = t;
//	intersection.hit = true;
//	intersection.pos = ray.pos + ray.dir * t;
//	intersection.n = intersection.pos - m_pos;
//	intersection.n.normalize();



	return intersection;




}
