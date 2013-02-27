#include "viewer.hpp"
#include "algebra.hpp"
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gtkmm/messagedialog.h>

#include "scene.hpp"

#include "debug.hpp"


/*******************************************************
 * 
 * void vCalcRotVec(float fNewX, float fNewY, 
 *                  float fOldX, float fOldY,
 *                  float fDiameter,
 *                  float *fVecX, float *fVecY, float *fVecZ);
 *
 *    Calculates a rotation vector based on mouse motion over
 *    a virtual trackball.
 *
 *    The fNew and fOld mouse positions
 *    should be in 'trackball' space. That is, they have been
 *    transformed into a coordinate system centered at the middle
 *    of the trackball. fNew, fOld, and fDiameter must all be specified
 *    in the same units (pixels for example).
 *
 * Parameters: fNewX, fNewY - New mouse position in trackball space.
 *                            This is the second point along direction
 *                            of rotation.
 *             fOldX, fOldY - Old mouse position in trackball space.
 *                            This is the first point along direction
 *                            of rotation.
 *             fDiameter - Diameter of the trackball. This should
 *                         be specified in the same units as fNew and fOld.
 *                         (ie, usually pixels if fNew and fOld are transformed
 *                         mouse positions)
 *             fVec - The output rotation vector. The length of the vector
 *                    is proportional to the angle of rotation.
 *
 *******************************************************/
void vCalcRotVec(float fNewX, float fNewY,
                 float fOldX, float fOldY,
                 float fDiameter,
                 float *fVecX, float *fVecY, float *fVecZ) {
  

   float fNewVecX, fNewVecY, fNewVecZ,        /* Vector corresponding to new mouse location */
         fOldVecX, fOldVecY, fOldVecZ,        /* Vector corresponding to old mouse location */
         fLength;

   /* Vector pointing from center of virtual trackball to
    * new mouse position
    */
   fNewVecX    = fNewX * 2.0 / fDiameter;
   fNewVecY    = fNewY * 2.0 / fDiameter;
   fNewVecZ    = (1.0 - fNewVecX * fNewVecX - fNewVecY * fNewVecY);

   /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
   if (fNewVecZ < 0.0) {
      fLength = sqrt(1.0 - fNewVecZ);
      fNewVecZ  = 0.0;
      fNewVecX /= fLength;
      fNewVecY /= fLength;
      DEBUG_MSG("inside trackball");
   } else {
      fNewVecZ = sqrt(fNewVecZ);
   }

   /* Vector pointing from center of virtual trackball to
    * old mouse position
    */
   fOldVecX    = fOldX * 2.0 / fDiameter;
   fOldVecY    = fOldY * 2.0 / fDiameter;
   fOldVecZ    = (1.0 - fOldVecX * fOldVecX - fOldVecY * fOldVecY);
 
   /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
   if (fOldVecZ < 0.0) {
      fLength = sqrt(1.0 - fOldVecZ);
      fOldVecZ  = 0.0;
      fOldVecX /= fLength;
      fOldVecY /= fLength;
   } else {
      fOldVecZ = sqrt(fOldVecZ);
   }

   /* Generate rotation vector by calculating cross product:
    * 
    * fOldVec x fNewVec.
    * 
    * The rotation vector is the axis of rotation
    * and is non-unit length since the length of a crossproduct
    * is related to the angle between fOldVec and fNewVec which we need
    * in order to perform the rotation.
    */
   *fVecX = fOldVecY * fNewVecZ - fNewVecY * fOldVecZ;
   *fVecY = fOldVecZ * fNewVecX - fNewVecZ * fOldVecX;
   *fVecZ = fOldVecX * fNewVecY - fNewVecX * fOldVecY;
}

/*******************************************************
 * void vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, Matrix mNewMat)
 *    
 *    Calculate the rotation matrix for rotation about an arbitrary axis.
 *    
 *    The axis of rotation is specified by (fVecX,fVecY,fVecZ). The length
 *    of the vector is the amount to rotate by.
 *
 * Parameters: fVecX,fVecY,fVecZ - Axis of rotation
 *             mNewMat - Output matrix of rotation in column-major format
 *                       (ie, translation components are in column 3 on rows
 *                       0,1, and 2).
 *
 *******************************************************/
Matrix4x4 vAxisRotMatrix(float fVecX, float fVecY, float fVecZ) {


    float fRadians, fInvLength, fNewVecX, fNewVecY, fNewVecZ;

    /* Find the length of the vector which is the angle of rotation
     * (in radians)
     */
    fRadians = sqrt(fVecX * fVecX + fVecY * fVecY + fVecZ * fVecZ);

    /* If the vector has zero length - return the identity matrix */
    if (fRadians > -0.000001 && fRadians < 0.000001) {
        
        return Matrix4x4();
    }

    /* Normalize the rotation vector now in preparation for making
     * rotation matrix. 
     */
    fInvLength = 1 / fRadians;
    fNewVecX   = fVecX * fInvLength;
    fNewVecY   = fVecY * fInvLength;
    fNewVecZ   = fVecZ * fInvLength;

    /* Create the arbitrary axis rotation matrix */
    double dSinAlpha = sin(fRadians);
    double dCosAlpha = cos(fRadians);
    double dT = 1 - dCosAlpha;

    Matrix4x4 mNewMat;

    mNewMat[0][0] = dCosAlpha + fNewVecX*fNewVecX*dT;
    mNewMat[0][1] = fNewVecX*fNewVecY*dT + fNewVecZ*dSinAlpha;
    mNewMat[0][2] = fNewVecX*fNewVecZ*dT - fNewVecY*dSinAlpha;
    mNewMat[0][3] = 0;



    mNewMat[1][0] = fNewVecX*fNewVecY*dT - dSinAlpha*fNewVecZ;
    mNewMat[1][1] = dCosAlpha + fNewVecY*fNewVecY*dT;
    mNewMat[1][2] = fNewVecY*fNewVecZ*dT + dSinAlpha*fNewVecX;
    mNewMat[1][3] = 0;

    mNewMat[2][0] = fNewVecZ*fNewVecX*dT + dSinAlpha*fNewVecY;
    mNewMat[2][1] = fNewVecZ*fNewVecY*dT - dSinAlpha*fNewVecX;
    mNewMat[2][2] = dCosAlpha + fNewVecZ*fNewVecZ*dT;
    mNewMat[2][3] = 0;

    mNewMat[3][0] = 0;
    mNewMat[3][1] = 0;
    mNewMat[3][2] = 0;
    mNewMat[3][3] = 1;

    return mNewMat;
}



Viewer::Viewer():
  m_root(NULL),
  m_mode(POSITION),
  m_drawCircle(false),
  m_frontCull(false),
  m_backCull(false),
  m_zBuffer(false)
{
  Glib::RefPtr<Gdk::GL::Config> glconfig;

  // Ask for an OpenGL Setup with
  //  - red, green and blue component colour
  //  - a depth buffer to avoid things overlapping wrongly
  //  - double-buffered rendering to avoid tearing/flickering
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                     Gdk::GL::MODE_DEPTH |
                                     Gdk::GL::MODE_DOUBLE);
  if (glconfig == 0) {
    // If we can't get this configuration, die
    std::cerr << "Unable to setup OpenGL Configuration!" << std::endl;
    abort();
  }

  // Accept the configuration
  set_gl_capability(glconfig);

  // Register the fact that we want to receive these events
  add_events(Gdk::BUTTON1_MOTION_MASK    |
             Gdk::BUTTON2_MOTION_MASK    |
             Gdk::BUTTON3_MOTION_MASK    |
             Gdk::BUTTON_PRESS_MASK      | 
             Gdk::BUTTON_RELEASE_MASK    |
             Gdk::VISIBILITY_NOTIFY_MASK);

  reset(RESET_ALL);
}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::reset(ResetType type) {
  DEBUG_MSG("reset all");

  bool resetPos = false, resetOri = false, resetJoint = false;
  switch(type) {
  case RESET_POSITION:
    resetPos = true;
    break;
  case RESET_ORIENTATION:
    resetOri = true;
    break;
  case RESET_JOINTS:
    resetJoint = true;
    break;
  case RESET_ALL:
    resetPos = true;
    resetOri = true;
    resetJoint = true;
    break;

  }

  if (resetPos) {

    m_trackballTranslation = translation(Vector3D(0.0, 0.0, -5.0));
  }
  if (resetOri) {

    m_trackballRotation = Matrix4x4();
  }
  if (resetJoint) {
    if (m_root) {
      m_root->resetJoints();
    }

    m_undo = HistoryStack();
    m_redo = HistoryStack();
    m_selected.clear();
  }

  m_lastMouse = Point2D();

  // invalidate other options?

  invalidate();

}


void Viewer::setMode(Mode mode) {
  m_mode = mode;
  DEBUG_MSG("SetMode: " << m_mode);
}
void Viewer::toggleOption(Option option) {
  DEBUG_MSG("ToggleOption: " << option);
  switch (option) {
  case CIRCLE:
    m_drawCircle = !m_drawCircle;
    DEBUG_MSG("Draw Circle: " << m_drawCircle);
    break;  
  
  case ZBUFFER:
    m_zBuffer = !m_zBuffer;
    break;
  case BACK_CULL:
    m_backCull = !m_backCull;
    break;
  case FRONT_CULL:
    m_frontCull = !m_frontCull;
    break;
  }

  invalidate();
}

void Viewer::undo() {

  if (m_undo.empty()) {
    Gtk::MessageDialog dialog("Can't undo");
    dialog.run();
    return;
  }

  History record = m_undo.top();
  m_undo.pop();

  // execute that record
  SceneNode *node = m_root->find(record.id);
  assert(node);
  assert(node->is_joint());
  JointNode *joint = dynamic_cast<JointNode*>(node);
  assert(joint);

  joint->joint_x() = record.xRot;
  joint->joint_y() = record.yRot;


  m_redo.push(record);
}

void Viewer::redo() {

  if (m_redo.empty()) {
    Gtk::MessageDialog dialog("Can't redo");
    dialog.run();
    return;
  }

  History record = m_redo.top();
  m_redo.pop();

    // execute that record
  SceneNode *node = m_root->find(record.id);
  assert(node);
  assert(node->is_joint());
  JointNode *joint = dynamic_cast<JointNode*>(node);
  assert(joint);

  joint->joint_x() = record.xRot;
  joint->joint_y() = record.yRot;


  m_undo.push(record);
}

void Viewer::invalidate()
{
  // Force a rerender
  Gtk::Allocation allocation = get_allocation();

  if (get_window()) {
    get_window()->invalidate_rect( allocation, false);
  }
}

void Viewer::on_realize()
{
  // Do some OpenGL setup.
  // First, let the base class do whatever it needs to
  Gtk::GL::DrawingArea::on_realize();
  
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
  
  if (!gldrawable)
    return;

  if (!gldrawable->gl_begin(get_gl_context()))
    return;

  glShadeModel(GL_SMOOTH);
  glClearColor( 0.4, 0.4, 0.4, 0.0 );
  glEnable(GL_DEPTH_TEST);


  gldrawable->gl_end();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  //glEnable(GL_CULL_FACE);
  //glCullFace(GL_FRONT);

// blending
  glEnable (GL_BLEND);
glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Set up for perspective drawing 
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, get_width(), get_height());
  gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.1, 1000.0);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Clear framebuffer{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Set up lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  // put light behind camera
  GLfloat lightpos[] = {0.5f, 1.f, 10.f, 0.f};
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

  GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.f};
  GLfloat diffuse[] = {0.8f, 0.8, 0.8f, 1.f};
  GLfloat specular[] = {1.f, 1.f, 1.f, 1.f};
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

  glEnable(GL_NORMALIZE);

  // toogle options
  DEBUG_MSG("Front/Back Cull " << m_frontCull << "/" << m_backCull << " Zbuf " << m_zBuffer);
  if (m_frontCull || m_backCull) {
    glEnable(GL_CULL_FACE);
    if (m_backCull && m_frontCull) {
      glCullFace(GL_FRONT_AND_BACK);
    }
    else if (m_frontCull) {
      glCullFace(GL_FRONT);
    }
    else {
      glCullFace(GL_BACK);
    }
  }
  else {
    glDisable(GL_CULL_FACE);
  }
  if (m_zBuffer) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
  }
  else {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
  }


  glPushMatrix();

    //Matrix4x4 daT = translation(Vector3D(5.0, 0.0, 0.0));
    //glMultMatrixd(daT.transpose().begin());

    // gl use column order matrix, m4x4 is row order
    Matrix4x4 trackballM = m_trackballTranslation * m_trackballRotation;
    glMultMatrixd(trackballM.transpose().begin());

      // draw model
    m_root->walk_gl(false);


    // glPushMatrix();

    //   // push keep into screen
    //   //glTranslated(0.0, 0.0, -10.0);

    //   //glTranslated(0.0, 0.0, -20.0);
    //   GLUquadricObj * quadric = gluNewQuadric();
    //   gluSphere(quadric, 1.f, 10, 10);

    // glPopMatrix();

  glPopMatrix();



  if (m_drawCircle) {
    draw_trackball_circle();
  }

  // Swap the contents of the front and back buffers so we see what we
  // just drew. This should only be done if double buffering is enabled.
  gldrawable->swap_buffers();

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;
  
  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Set up perspective projection, using current size and aspect
  // ratio of display

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, event->width, event->height);
  gluPerspective(40.0, (GLfloat)event->width/(GLfloat)event->height, 0.1, 1000.0);

  // Reset to modelview matrix mode
  
  glMatrixMode(GL_MODELVIEW);

  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
  DEBUG_MSG( "Stub: Button " << event->button << " pressed" );


  m_lastMouse = Point2D(event->x, event->y);

  // pick here
  if (m_mode == JOINTS && event->button == 1) {

    GLuint buffer[512];
    GLint viewport[4];
    glSelectBuffer (512, buffer);

    // don't actually draw
    glRenderMode(GL_SELECT);
    glInitNames();  //init the name buffer

    GLint hits;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    //glViewport(0, 0, get_width(), get_height
    glGetIntegerv(GL_VIEWPORT, viewport);
    for (int i = 0; i < 4; i++) {

      DEBUG_MSG("Viewport[i]: " << viewport[i]);
    }

    gluPickMatrix(event->x, viewport[3] - event->y, 1, 1, viewport);
    gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.1, 1000.0);

  // change to model view for drawing
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();

    //Matrix4x4 daT = translation(Vector3D(5.0, 0.0, 0.0));
    //glMultMatrixd(daT.transpose().begin());

    // gl use column order matrix, m4x4 is row order
    Matrix4x4 trackballM = m_trackballTranslation * m_trackballRotation;
    glMultMatrixd(trackballM.transpose().begin());

      // draw model
    m_root->walk_gl(true);

    glPopMatrix();

    hits = glRenderMode(GL_RENDER);

    DEBUG_MSG("picked: " << hits << "==========================");

    //parse the hits
    int c = 0;  // buffer counter
    for (int h = 0; h < hits; h++) {
      int qStack = buffer[c++];
      int minZ = buffer[c++];
      int maxZ = buffer[c++];
      DEBUG_MSG("HIT(" << h << ")" << qStack << " (" << minZ << "," << maxZ << ")");
      for (int q = 0; q < qStack; q++) {
        int id = buffer[c++];
        DEBUG_MSG("\tname " << id);

        SceneNode *selNode = m_root->find(id);
        if (selNode == NULL) {
                  *(int*)0=0;
                  DEBUG_MSG("IMPOSSIBLE");
                  exit(-1);
        }

        // check if selNode is selectable (has a joint parent)
        if (selNode->jointParent()) {

          bool selected = selNode->toggleSelected();
          if (selected) {
            m_selected.push_back(selNode);
          }
          else {
            m_selected.remove(selNode);
          }
        }
      }

    }

    // select the primitives if they have joint owners
    // when dragging, move the joints of the parents

    // able to get GemeotryNode from id
      // tree or global arraylist

    // get parent from child
  }

  invalidate();
  return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
  DEBUG_MSG(" Stub: Button " << event->button << " released");

  // save state
  if (m_mode == JOINTS) {

    // save to undo stack

  }

  invalidate();

  return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
  DEBUG_MSG("Stub: Motion at " << event->x << ", " << event->y );

  Point2D dMouse(event->x - m_lastMouse[0], event->y - m_lastMouse[1]);

  bool b1,b2,b3;
  b1 = event->state & GDK_BUTTON1_MASK;
  b2 = event->state & GDK_BUTTON2_MASK;
  b3 = event->state & GDK_BUTTON3_MASK;

  //track ball mode
  if (m_mode == POSITION) {
    // do x,y translation
    if (b1) {

      double translationScale = 50.0;

      Vector3D translate(dMouse[0] / translationScale, -dMouse[1] / translationScale , 0);

      Matrix4x4 t = translation(translate);
      m_trackballTranslation = m_trackballTranslation * t;

    }

    if (b2) {

      Vector3D translate(0.0, 0.0, dMouse[1]/ 35.0);

      Matrix4x4 t = translation(translate);
      m_trackballTranslation = m_trackballTranslation * t;
    }

    if (b3) {
      /*
       * Track ball rotations are being used.
       */
     
          float fDiameter;
          int iCenterX, iCenterY;
          float fNewModX, fNewModY, fOldModX, fOldModY;
    
          /* vCalcRotVec expects new and old positions in relation to the center of the
           * trackball circle which is centered in the middle of the window and
           * half the smaller of nWinWidth or nWinHeight.
           */
          fDiameter = (get_width() < get_height()) ? (float)get_width() * 0.5f : (float)get_height() * 0.5f;
          iCenterX = get_width() / 2;
          iCenterY = get_height() / 2;


          fOldModX = m_lastMouse[0] - iCenterX;
          fOldModY = m_lastMouse[1] - iCenterY;
          fNewModX = event->x - iCenterX;
          fNewModY = event->y - iCenterY;


      float  fRotVecX, fRotVecY, fRotVecZ;

          vCalcRotVec(fNewModX, fNewModY,
                          fOldModX, fOldModY,
                          fDiameter,
                          &fRotVecX, &fRotVecY, &fRotVecZ);
          /* Negate Y component since Y axis increases downwards
           * in screen space and upwards in OpenGL.
           */
          Matrix4x4 mNewMat = vAxisRotMatrix(fRotVecX, -fRotVecY, fRotVecZ);

          // Since all these matrices are meant to be loaded
          // into the OpenGL matrix stack, we need to transpose the
          // rotation matrix (since OpenGL wants rows stored
          // in columns)

          //vTransposeMatrix(mNewMat);
          //vRightMultiply(mRotations, mNewMat);

          m_trackballRotation = m_trackballRotation * mNewMat;
      }
    


    DEBUG_MSG("trackballMatrix: Translation\n" << m_trackballTranslation <<
      "Rotation\n" << m_trackballRotation);
  }
  else {
    // JOINT manipulation mode

    // rotate head up down relative to y when b2
    // rotate head left right with x motion when b3

    // grab all selected nodes
    // grab their parents
    // rotate their joint left/right, up/down

     for (std::list<SceneNode*>::const_iterator it = m_selected.begin(), end = m_selected.end(); it != end; ++it) {
     
      assert((*it)->is_selected());
      
      SceneNode *parent = (*it)->jointParent();

      // check if parent is joint node
      
      DEBUG_MSG("Rotating: " << (*it)->name());
      assert(parent);
      DEBUG_MSG("Parent: name " << parent->name());
      assert(parent->is_joint());
      JointNode *joint = dynamic_cast<JointNode*>(parent);
      assert(joint);

      double rotationScale = -3.0;
      
      if (b2) {
        // rotate in degrees
        joint->rotate( dMouse[1] / rotationScale, 0.0);
      }

      if (b3) {
        joint->rotate(0.0, dMouse[0] / rotationScale);
      }


    }

  }

  m_lastMouse = Point2D(event->x, event->y);

  invalidate();
  return true;
}

void Viewer::draw_trackball_circle()
{
  int current_width = get_width();
  int current_height = get_height();
  
  // Set up for orthogonal drawing to draw a circle on screen.
  // You'll want to make the rest of the function conditional on
  // whether or not we want to draw the circle this time around.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, current_width, current_height);
  glOrtho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);

  // Reset modelview matrix
  glLoadIdentity();

  // draw a circle for use with the trackball 
  glDisable(GL_LIGHTING);
  glEnable(GL_LINE_SMOOTH);
  glColor3f(1.0, 1.0, 1.0);
  double radius = current_width < current_height ? 
    (float)current_width * 0.25 : (float)current_height * 0.25;
  glTranslated((float)current_width * 0.5, (float)current_height * 0.5, 0);
  glBegin(GL_LINE_LOOP);
  for(size_t i=0; i<40; ++i) {
    double cosine = radius * cos(i*2*M_PI/40);
    double sine = radius * sin(i*2*M_PI/40);
    glVertex2f(cosine, sine);
  }
  glEnd();
  glColor3f(0.0, 0.0, 0.0);
  glDisable(GL_LINE_SMOOTH);
}

void Viewer::setSceneRoot(SceneNode *root) {
  m_root = root;

}
