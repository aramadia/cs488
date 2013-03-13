#include "primitive.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Intersection Sphere::intersect(Ray ray) {
	Intersection intersection;

	return intersection;
}


Cube::~Cube()
{
}

Intersection Cube::intersect(Ray ray) {
	Intersection intersection;

	return intersection;
}

NonhierSphere::~NonhierSphere()
{
}


Intersection NonhierSphere::intersect(Ray ray) {

// float NonhierSphere::intersect(Vector3D rayOrigin, Vector3D rayDirection, 
//     Vector3D &out_intersection, Vector3D &out_normal) {

	Intersection intersection;

	Vector3D dist = Vector3D(m_pos) - ray.pos; 
	double B = ray.dir.dot( dist);
	double D = B*B - dist.dot( dist) + m_radius * m_radius; 

	if (D < 0.0f)  {
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
}

Intersection NonhierBox::intersect(Ray ray) {
	Intersection intersection;

	return intersection;
}
