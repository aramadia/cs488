#include "primitive.hpp"

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}


float NonhierSphere::intersect(Vector3D rayOrigin, Vector3D rayDirection) {
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
