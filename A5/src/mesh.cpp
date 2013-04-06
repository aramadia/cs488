#include "mesh.hpp"
#include <iostream>

#include "debug.hpp"

Mesh::Mesh(const std::vector<Point3D>& verts,
           const std::vector< std::vector<int> >& faces)
  : m_verts(verts),
    m_faces(faces)
{
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin()) std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";
  
  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin()) std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin()) std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}

// Based of http://stackoverflow.com/questions/312328/what-is-the-fastest-way-to-find-the-point-of-intersection-between-a-ray-and-a-po
Intersection intersectPolygon(Ray ray, std::vector<Point3D> polygon)
{
  Intersection intersection;

  assert(polygon.size() >= 3);
  Vector3D planeNormal = (polygon[2] - polygon[1]).cross(polygon[0] - polygon[1]);
  planeNormal.normalize();

  // ray-plane intersection. r = n*(V0-P0)/(n*(P1-P0))
  // p0-p1 = ray, v0 = point on plane, n plane normal

  double den = planeNormal.dot(ray.dir);
  if (den < 0.001)
  {
    return intersection;
  }

  double num = planeNormal.dot(polygon[0] - ray.pos);
  double t = num/den;
  if (t < 0.001)
  {
	  return intersection;
  }

  // DEBUG_MSG("plane hit: " << t);

  intersection.pos = ray.pos + t*ray.dir;

  bool point_is_inside = false;
  for (unsigned int i = 0; i < polygon.size(); i++)
  {
	  double d = ((polygon[(i+1)%polygon.size()]) - polygon[i]).cross(intersection.pos - polygon[i]).dot(planeNormal);
	  if (d >= 0)
	  {
		  point_is_inside = true;
	  }
	  else
	  {
		  point_is_inside = false;
		  break;
	  }

  }

//  // ok....
//  // point test_line = vectorSub(Answer, Poly.P[0])
//  // point test_axis = crossProduct(plane_normal, test_line)
//
//  Vector3D test_line = intersection.pos - polygon[0];
//  Vector3D test_axis = planeNormal.cross(test_line);
//
//  bool point_is_inside = false;
//
//  // point test_point = vectorSub(Poly.P[1], Answer)
//  // bool prev_point_ahead = (dotProduct(test_line, test_point) > 0)
//  // bool prev_point_above = (dotProduct(test_axis, test_point) > 0)
//
//  Vector3D test_point = polygon[1] - intersection.pos;
//  bool prev_point_ahead = test_line.dot(test_point) > 0;
//  bool prev_point_above = test_axis.dot(test_point) > 0;
//
//
//
//  bool this_point_ahead;
//  bool this_point_above;
//
//  unsigned int index = 2;
//  while (index < polygon.size())
//  {
//      // test_point = vectorSub(Poly.P[index], Answer)
//      // this_point_ahead = (dotProduct(test_line, test_point) > 0)
//
//      test_point = polygon[index] - intersection.pos;
//      this_point_ahead = test_line.dot(test_point) > 0;
//
//      if (prev_point_ahead || this_point_ahead)
//      {
//          //this_point_above = (dotProduct(test_axis, test_point) > 0)
//        this_point_above = test_axis.dot(test_point) > 0;
//
//        if (prev_point_above ^ this_point_above)
//        {
//            point_is_inside = !point_is_inside;
//        }
//      }
//
//      prev_point_ahead = this_point_ahead;
//      prev_point_above = this_point_above;
//      index++;
//    }

  if (point_is_inside)
  {
    intersection.hit =  point_is_inside;
    intersection.n = -1.0 * planeNormal;

    intersection.t = t;
    //DEBUG_MSG("Hit mesh " << t);
  }
    return intersection;

}


Intersection Mesh::intersect(Ray ray) {
  Intersection intersection;

  //DEBUG_MSG("Vxs " << m_verts.size() << " Faces: " << m_faces.size() );
  // intersect ray polygon
  //for (SceneNode::ChildList::const_iterator it = root->children().begin(), end = root->children().end(); it != end; ++it) {
  for (std::vector<Face>::const_iterator it = m_faces.begin(), end = m_faces.end(); it != end; ++it)
  {
    std::vector<Point3D> polygon;

    Face face = *it;
    for (unsigned int i = 0; i < face.size(); i++)
    {
      polygon.push_back(m_verts[face[i]]);

      //DEBUG_MSG("\t" << face[i]);
    }
    //DEBUG_MSG(polygon.size());

    Intersection pIntersect = intersectPolygon(ray, polygon);

    if (pIntersect.hit)
    {
      //DEBUG_MSG("HIT");
      if (!intersection.hit || pIntersect.t < intersection.t )
      {

        //DEBUG_MSG("HIT SET");
        intersection = pIntersect;
      }
    }


  }    


  return intersection;
}
