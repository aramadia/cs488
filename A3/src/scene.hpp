#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"

class SceneNode {
public:
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  virtual void walk_gl(bool picking = false) const;

  const Matrix4x4& get_transform() const { return m_trans; }
  const Matrix4x4& get_inverse() const { return m_invtrans; }
  
  void set_transform(const Matrix4x4& m)
  {
    m_trans = m;
    m_invtrans = m.invert();
  }

  void set_transform(const Matrix4x4& m, const Matrix4x4& i)
  {
    m_trans = m;
    m_invtrans = i;
  }

  SceneNode *find(int id);

  void add_child(SceneNode* child)
  {
    m_children.push_back(child);
    child->m_parent = this;
  }

  void remove_child(SceneNode* child)
  {
    m_children.remove(child);
    child->m_parent = NULL;
  }

  // Callbacks to be implemented.
  // These will be called from Lua.
  void rotate(char axis, double angle);
  void scale(const Vector3D& amount);
  void translate(const Vector3D& amount);

  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;

  bool toggleSelected();

  SceneNode *parent() { return m_parent; }
  SceneNode *jointParent();
  bool is_selected() { return m_selected; }

  // reset all joints underneath this node
  virtual void resetJoints();

  std::string name() const { return m_name; }
  
protected:
  static int s_idCounter;
  
  // Useful for picking
  int m_id;
  std::string m_name;

  bool m_selected;

  // Transformations
  Matrix4x4 m_trans;
  Matrix4x4 m_invtrans;

  SceneNode *m_parent;

  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;
};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual void walk_gl(bool bicking = false) const;

  virtual bool is_joint() const;

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  void rotate(double xDeg, double yDeg);
  virtual void resetJoints();

  struct JointRange {
    double min, init, max, cur;
  };


  double &joint_x() { return m_joint_x.cur; }
  double &joint_y() { return m_joint_y.cur; }

  
protected:

  JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  virtual void walk_gl(bool picking = false) const;

  const Material* get_material() const;
  Material* get_material();

  void set_material(Material* material)
  {
    m_material = material;
  }

protected:
  Material* m_material;
  Primitive* m_primitive;
};

#endif
