#include "scene.hpp"
#include <iostream>


#include <GL/gl.h>
#include <GL/glu.h>


#include "debug.hpp"

int SceneNode::s_idCounter = 0;
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
    m_selected(false)
{
  m_id = s_idCounter++;
  DEBUG_MSG("SceneNode::Created node(" << m_id << ") " << m_name);
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking) const
{
  DEBUG_MSG("SceneNode::walk_gl: " << m_name);

  glPushMatrix();
  glMultMatrixd(m_trans.transpose().begin());


  for (ChildList::const_iterator it = m_children.begin(), end = m_children.end(); it != end; ++it) {
   // std::cout << *iterator;
    
    (*it)->walk_gl(picking);

  }


  glPopMatrix();
}

SceneNode *SceneNode::find(int id) {
  if (m_id == id) {
    return this;
  }
  for (ChildList::const_iterator it = m_children.begin(), end = m_children.end(); it != end; ++it) {
   // std::cout << *iterator;
    
    SceneNode *node = (*it)->find(id);
    if (node) {
      return node;
    }

  }
  return NULL;
}

bool SceneNode::toggleSelected() {

  m_selected = !m_selected;
  return m_selected;
}

void SceneNode::rotate(char axis, double angle)
{
  DEBUG_MSG( "SceneNode:Rotate " << m_name << " around " << axis << " by " << angle );
  // Fill me in
  //rotation(double angle, char axis)

  // convert to radians
  angle = angle * M_PI/ 180.0;

  Matrix4x4 r = rotation(angle, axis);
  set_transform(m_trans * r);
}

void SceneNode::scale(const Vector3D& amount)
{
  DEBUG_MSG( "SceneNode: Scale " << m_name << " by " << amount );
  Matrix4x4 s = scaling(amount);
  set_transform(m_trans * s);


}

void SceneNode::translate(const Vector3D& amount)
{
  DEBUG_MSG( "SceneNode:Translate " << m_name << " by " << amount );
  // Fill me in

  Matrix4x4 t = translation(amount);
  set_transform(m_trans * t);
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking) const
{
  // Fill me in
  DEBUG_MSG("JOINT NODE::walkgl NIMPL");
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

void GeometryNode::walk_gl(bool picking) const
{


  DEBUG_MSG("GeometryNode::walk_gl: " << m_name);

  // Fill me in

  // transform the coordinates
  glPushMatrix();
    glPushName(m_id);
    glMultMatrixd(m_trans.transpose().begin());

    // apply material
    if (!m_selected) {
      m_material->apply_gl();
    }
    else {
      // apply default selected material
      GLfloat diffuse[] = {0.f, 1.f, 1.f, 1.0f};
      GLfloat specular[] = {1.f, 1.f, 1.f, 1.f};
      GLfloat shininess[] = {10};


      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    }

    // pick


    // draw primitive
    m_primitive->walk_gl(picking);
    glPopName();
  glPopMatrix();
}
 
