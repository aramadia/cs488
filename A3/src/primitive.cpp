#include "primitive.hpp"

#include "debug.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl(bool picking) const
{

  DEBUG_MSG("Sphere::walk_gl:");

	// draw the sphere
  GLUquadricObj * quadric = gluNewQuadric();

	// gluQuadricNormals( quadric, GLU_SMOOTH );
	// gluQuadricTexture( quadric, GL_TRUE );
	// gluQuadricDrawStyle( quadric, GLU_FILL );
  gluSphere(quadric, 1.f, 10, 10);
}
