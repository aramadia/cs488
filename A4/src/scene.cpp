#include "scene.hpp"
#include <iostream>

#include "debug.hpp"

SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

// From A3

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
Matrix4x4 rotation(double angle, char axis)
{
  Matrix4x4 r;

  Vector4D r1,r2,r3,r4;

  // convert to radians
  angle = angle * M_PI/ 180.0;

  double c = cos(angle);
  double s = sin(angle);

  switch(axis) {
    case 'x':
      r1 = Vector4D(1, 0, 0, 0);
      r2 = Vector4D(0, c, -s, 0);
      r3 = Vector4D(0, s, c, 0);
      r4 = Vector4D(0, 0, 0, 1);
      break;
  case 'y':

      r1 = Vector4D(c, 0, s, 0);
      r2 = Vector4D(0, 1, 0, 0);
      r3 = Vector4D(-s, 0, c, 0);
      r4 = Vector4D(0, 0, 0, 1);
      break;
  case 'z':
      r1 = Vector4D(c, -s, 0, 0);
      r2 = Vector4D(s, c, 0, 0);
      r3 = Vector4D(0, 0, 1, 0);
      r4 = Vector4D(0, 0, 0, 1);
      break;
  default:
    DEBUG_MSG("Unknown angle" << axis);
    assert(false);


  }

  r = Matrix4x4(r1,r2,r3,r4);
  return r;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement)
{
  Matrix4x4 t;
  // Fill me in!

  double x = displacement[0];
  double y = displacement[1];
  double z = displacement[2];

  Vector4D r1(1, 0, 0, x);
  Vector4D r2(0, 1, 0, y);
  Vector4D r3(0, 0, 1, z);
  Vector4D r4(0, 0, 0, 1);

  t = Matrix4x4(r1,r2,r3,r4);

  return t;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale)
{
  Matrix4x4 s;


  double x = scale[0];
  double y = scale[1];
  double z = scale[2];

  Vector4D r1(x, 0, 0, 0);
  Vector4D r2(0, y, 0, 0);
  Vector4D r3(0, 0, z, 0);
  Vector4D r4(0, 0, 0, 1);

  s = Matrix4x4(r1,r2,r3,r4);


  return s;
}



void SceneNode::rotate(char axis, double angle)
{
	DEBUG_MSG(" Rotate " << m_name << " around " << axis << " by " << angle);

  Matrix4x4 r = rotation(angle, axis);
  m_trans = m_trans * r;
  m_invtrans = m_trans.invert();
}

void SceneNode::scale(const Vector3D& amount)
{
  DEBUG_MSG(" Scale " << m_name << " by " << amount );

  Matrix4x4 s = scaling(amount);
  m_trans = m_trans * s;
  m_invtrans = m_trans.invert();
}

void SceneNode::translate(const Vector3D& amount)
{
	DEBUG_MSG(" Translate " << m_name << " by " << amount );

  Matrix4x4 t = translation(amount);
  m_trans = m_trans * t;
  m_invtrans = m_trans.invert();
}

bool SceneNode::is_joint() const
{
  return false;
}

Intersection SceneNode::intersect(Ray ray)
{
	Intersection intersection;

	// transform ray.  Instead of moving the node, move the ray by the inverse transform
	DEBUG_MSG("Original ray: " << ray.pos << "," << ray.dir << " at " << m_name << "");
	ray.dir = m_invtrans * ray.dir;
	ray.pos = m_invtrans * ray.pos;
	DEBUG_MSG("Transformed ray: " << ray.pos << "," << ray.dir << " at " << m_name << "");

//	Matrix4x4 t = translation(Vector3D(150, 0, 0));
//	DEBUG_MSG("Translation " << t);
//	ray.pos =  t * ray.pos;
//	DEBUG_MSG("Test ray: " << ray.pos << "," << ray.dir << " at " << m_name << "");

	for (SceneNode::ChildList::const_iterator it = m_children.begin(), end = m_children.end();
						it != end; ++it)
	{
		//DEBUG_MSG((*it)->name());
		SceneNode *node = *it;

		Intersection i = node->intersect(ray);

		if (i.hit)
		{
			assert(i.t > 0);
			if (!intersection.hit || i.t < intersection.t)
			{
				intersection = i;
			}
		}

	}

	// untransform the intersection
	if (intersection.hit)
	{
		intersection.pos = m_trans * intersection.pos;
		intersection.n = m_invtrans.transpose() * intersection.n;
	}


	return intersection;

}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
}

GeometryNode::~GeometryNode()
{
}
 

Intersection GeometryNode::intersect(Ray ray)
{
	// transform ray.  Instead of moving the node, move the ray by the inverse transform
	ray.dir = m_invtrans * ray.dir;
	ray.pos = m_invtrans * ray.pos;

	Intersection intersection = m_primitive->intersect(ray);
	if (intersection.hit)
	{
		intersection.mat = m_material;
	}


	//TODO
	assert(m_children.size() == 0);
	for (SceneNode::ChildList::const_iterator it = m_children.begin(), end = m_children.end();
						it != end; ++it)
	{

		//DEBUG_MSG((*it)->name());
		SceneNode *node = *it;

		Intersection i = node->intersect(ray);

		if (i.hit)
		{
			assert(i.t > 0);
			if (!intersection.hit || i.t < intersection.t)
			{
				intersection = i;
			}
		}

	}

	// untransform the intersection
	if (intersection.hit)
	{
		intersection.pos = m_trans * intersection.pos;
		intersection.n = m_invtrans.transpose() * intersection.n;
	}


	return intersection;

}
