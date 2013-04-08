#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"

class Mesh;

class Primitive {
public:
  virtual ~Primitive();

  virtual Intersection intersect(Ray ray) = 0;
};



class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual Intersection intersect(Ray ray);

private:
  Point3D m_pos;
  double m_radius;
};

// Box is really a mesh
class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size);
  
  virtual ~NonhierBox();
  virtual Intersection intersect(Ray ray);

private:
  Point3D m_pos;
  double m_size;
  Mesh *m_mesh;
};


// Box is really a mesh
class NonhierFractal : public Primitive {
public:
	NonhierFractal(const Point3D& pos, double size);

  virtual ~NonhierFractal();
  virtual Intersection intersect(Ray ray);

private:
  Point3D m_pos;
  double m_size;
};

class Sphere : public Primitive {
public:
	Sphere();
  virtual ~Sphere();
  virtual Intersection intersect(Ray ray);
private:
  NonhierSphere *m_sphere;
};

class Cube : public Primitive {
public:
	Cube();
  virtual ~Cube();
  virtual Intersection intersect(Ray ray);
private:
  NonhierBox *m_cube;
};

#endif
