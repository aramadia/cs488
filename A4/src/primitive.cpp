#include "primitive.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}


float NonhierSphere::intersect(Vector3D rayOrigin, Vector3D rayDirection, 
    Vector3D &out_intersection, Vector3D &out_normal) {

	Vector3D dist = Vector3D(m_pos) - rayOrigin; 
	double B = rayDirection.dot( dist);
	double D = B*B - dist.dot( dist) + m_radius * m_radius; 

	if (D < 0.0f) 
	    return -1; 
	double t0 = B - sqrt(D); 
	double t1 = B + sqrt(D);
	double retValue = -1;  

	if ((t0 > 0.1f) ) 
	{
	    
	    retValue = t0;

	} 
	if ((t1 > 0.1f) && (t1 < retValue)) 
	{
	    retValue = t1;
	}


	if (retValue > 0) {
		out_intersection = rayOrigin + rayDirection * retValue;
		out_normal = out_intersection - m_pos;
		out_normal.normalize();
	}

	return retValue;
}

Cube::~Cube()
{
}

NonhierSphere::~NonhierSphere()
{
}

NonhierBox::~NonhierBox()
{
}
