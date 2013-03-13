#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"

class Primitive {
public:
  virtual ~Primitive();

  virtual Intersection intersect(Ray ray) = 0;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual Intersection intersect(Ray ray);
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  virtual Intersection intersect(Ray ray);
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

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();
  virtual Intersection intersect(Ray ray);

private:
  Point3D m_pos;
  double m_size;
};

#endif
