#include "scene.hpp"
#include <iostream>
#include <algorithm>

#include <GL/gl.h>
#include <GL/glu.h>


#include "debug.hpp"

PhongMaterial selectedMaterial(Colour(0.f, 1.f, 1.f),
                                     Colour(1.f, 1.f, 1.f),
                                     80);
PhongMaterial defaultMaterial(Colour(1.f, 0.f, 0.f),
                                     Colour(1.f, 0.5f, 0.5f),
                                     80);

PhongMaterial jointDebugMaterial(Colour(1.f, 0.f, 0.f),
                                     Colour(1.f, 1.f, 1.f),
                                     50);

int SceneNode::s_idCounter = 0;
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
    m_selected(false),
    m_parent(NULL)
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

SceneNode *SceneNode::jointParent() {

  // find the first parent thats a joint

  SceneNode *cur = parent();
  while (cur) {
    if (cur->is_joint()) return cur;
    cur = cur->parent();
    
  }
  return NULL;
}


void SceneNode::resetJoints() {
      
  m_selected = false;
  for (ChildList::const_iterator it = m_children.begin(), end = m_children.end(); it != end; ++it) {
    (*it)->resetJoints();
  }

}

void JointNode::resetJoints() {
 
  m_joint_x.cur = m_joint_x.init;
  m_joint_y.cur = m_joint_y.cur;
  
  m_selected = false;

  for (ChildList::const_iterator it = m_children.begin(), end = m_children.end(); it != end; ++it) {
    (*it)->resetJoints();
  }



}


void SceneNode::rotate(char axis, double angle)
{
  DEBUG_MSG( "SceneNode:Rotate " << m_name << " around " << axis << " by " << angle );
  // Fill me in
  //rotation(double angle, char axis)

  

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
  DEBUG_MSG("Joint Node::walk_gl: " << m_name);

  glPushMatrix();

    

    // create matrix for x and y direction
    Matrix4x4 finalTrans = m_trans * rotation(m_joint_x.cur, 'x') * rotation(m_joint_y.cur, 'y');
    glMultMatrixd(finalTrans.transpose().begin());

    // draw the joint for fun and profit
    // jointDebugMaterial.apply_gl();
    //   GLUquadricObj * quadric = gluNewQuadric();

    // gluQuadricNormals( quadric, GLU_SMOOTH );
    // gluQuadricTexture( quadric, GL_TRUE );
    // gluQuadricDrawStyle( quadric, GLU_FILL );
    // gluSphere(quadric, 0.25f, 30, 30);

    for (ChildList::const_iterator it = m_children.begin(), end = m_children.end(); it != end; ++it) {
      (*it)->walk_gl(picking);
    }

  glPopMatrix();
}

void JointNode::rotate(double xDeg, double yDeg) {
  m_joint_x.cur += xDeg;
  m_joint_y.cur += yDeg;

  // cap min and max
  m_joint_x.cur = std::max(std::min(m_joint_x.max, m_joint_x.cur), m_joint_x.min);
  m_joint_y.cur = std::max(std::min(m_joint_y.max, m_joint_y.cur), m_joint_y.min);
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
  m_joint_x.cur = init;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
  m_joint_y.cur = init;
}



GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
  m_material = &defaultMaterial;
}

GeometryNode::~GeometryNode()
{
}



void GeometryNode::walk_gl(bool picking) const
{
  DEBUG_MSG("GeometryNode::walk_gl: " << m_name);

  // transform the coordinates
  glPushMatrix();
    if (picking) {
      glPushName(m_id);
    }
    glMultMatrixd(m_trans.transpose().begin());

    // apply material
    if (!m_selected) {
      m_material->apply_gl();
    }
    else {
      // apply default selected material
      selectedMaterial.apply_gl();
    }

    // draw primitive
    m_primitive->walk_gl(picking);
    if (picking) {

      glPopName();
    }
  glPopMatrix();
}
 
