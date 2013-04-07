#include "primitive.hpp"
#include <vector>

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
