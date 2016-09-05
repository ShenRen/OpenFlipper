/*===========================================================================*\
 *                                                                           *
 *                              OpenFlipper                                  *
 *           Copyright (c) 2001-2016, RWTH-Aachen University                 *
 *           Department of Computer Graphics and Multimedia                  *
 *                          All rights reserved.                             *
 *                            www.openflipper.org                            *
 *                                                                           *
 *---------------------------------------------------------------------------*
 * This file is part of OpenFlipper.                                         *
 *---------------------------------------------------------------------------*
 *                                                                           *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 * 1. Redistributions of source code must retain the above copyright notice, *
 *    this list of conditions and the following disclaimer.                  *
 *                                                                           *
 * 2. Redistributions in binary form must reproduce the above copyright      *
 *    notice, this list of conditions and the following disclaimer in the    *
 *    documentation and/or other materials provided with the distribution.   *
 *                                                                           *
 * 3. Neither the name of the copyright holder nor the names of its          *
 *    contributors may be used to endorse or promote products derived from   *
 *    this software without specific prior written permission.               *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED *
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           *
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER *
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        *
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    *
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      *
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              *
 *                                                                           *
\*===========================================================================*/


//=============================================================================
//
//  CLASS PrincipalAxisNode - IMPLEMENTATION
//
//=============================================================================


//== INCLUDES =================================================================

#include "PrincipalAxisNode.hh"
#include <ACG/GL/gl.hh>

#include <ACG/QtWidgets/QtPrincipalAxisDialog.hh>

//== NAMESPACES ===============================================================

namespace ACG {
namespace SceneGraph {


//== IMPLEMENTATION ========================================================== 


// void 
// MyGlutPrimitiveNode::
// initialize()
// {
//   std::cerr << "initialize MyGlutPrimitiveNode static members!\n";
//   update_display_lists();
//   initialized_ = true;
// }



/// Default constructor
PrincipalAxisNode::PrincipalAxisNode( BaseNode*         _parent,
				      std::string       _name)
  : BaseNode(_parent, _name),
    auto_range_(false),
    max_abs_value_(1.0),
    min_abs_value_(0.0),
    max_draw_radius_(1.0),
    min_draw_radius_(0.0),
    default_radius_(true),
    slices_(10),
    cylinder_radius_scale_(1.0),
    min_spacing_(0.0),
    draw_style_(DS_3D),
    color_mode_(CM_Axis) {

  static const Vec4f default_cols[3] = {
          Vec4f(0.91, 0.11, 0.09, 1.0),
          Vec4f(0.0, .43, 1.0, 1.0),
          Vec4f(0.0, 0.70, 0.0, 1.0)
  };
  for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 4; ++j) {
          axes_colors[i][j] = default_cols[i][j];
      }
  }

  for(unsigned int i=0; i<3; ++i)
    show_tensor_component_[i] = 2;

  // create new quadric object
  qobj = gluNewQuadric();
}


//----------------------------------------------------------------------------


PrincipalAxisNode::~PrincipalAxisNode()
{
  // delete quadric object
  gluDeleteQuadric( qobj );
}

//----------------------------------------------------------------------------

void 
PrincipalAxisNode::
show_options_dialog()
{
  QtPrincipalAxisDialog* pd = new QtPrincipalAxisDialog(*this);
  pd->show();
}


//----------------------------------------------------------------------------


void 
PrincipalAxisNode::
show_tensor_component(unsigned int _i, unsigned char _show)
{
  if (_i > 2) return;

  show_tensor_component_[_i] = _show;

  auto_update_range();
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
resize(unsigned int _n)
{
  unsigned int old_n = pc_.size();

  pc_.resize(_n);
  draw_pc_.resize(_n);

  // initialize new draw_pc_ values
  for(unsigned int i=old_n; i<_n; ++i)
    draw_pc_[i] = false;

  auto_update_range();
}


//----------------------------------------------------------------------------


void 
PrincipalAxisNode::
enable ( unsigned int _i)
{
  if(_i < draw_pc_.size())
  {
    draw_pc_[_i] = true;
  }
  else std::cerr << "principal component index out of range\n";
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
disable ( unsigned int _i)
{
  if(_i < draw_pc_.size())
  {
    draw_pc_[_i] = false;
  }
  else std::cerr << "principal component index out of range\n";
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
disable_all()
{
  for(unsigned int i=0; i<pc_.size(); ++i)
    disable(i);
}


//----------------------------------------------------------------------------


void 
PrincipalAxisNode::
set(unsigned int _i, const PrincipalComponent& _pc)
{
  if( _i <= pc_.size())
  {
    // set values
    pc_[_i] = _pc;
    
    // update range
    if( auto_range_)
      auto_update_range();

//     // update bounding_box
//     update_bounding_box();
  }
  else std::cerr << "PrincipalComponent index error!\n";
}


//----------------------------------------------------------------------------


void 
PrincipalAxisNode::
get(unsigned int _i, PrincipalComponent& _pc)
{
  if( _i <= pc_.size())
  {
    // set values
    _pc = pc_[_i];
  }
  else std::cerr << "PrincipalComponent index error!\n";
}

//----------------------------------------------------------------------------


void
PrincipalAxisNode::
add(const PrincipalComponent& _pc, bool _enable)
{
  pc_.push_back(_pc);
  draw_pc_.push_back(_enable);

  // update range
  if( auto_range_)
    auto_update_range();
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
set_auto_range( bool _b)
{
  auto_range_ = _b;
}


//----------------------------------------------------------------------------


void 
PrincipalAxisNode::
set_min_abs_value( double _v)
{
  if( auto_range_)
    std::cerr << "Warning: Auto update min/max abs_values is enabled! Setting has no effect.\n";

  min_abs_value_ = _v;
}


//----------------------------------------------------------------------------


void 
PrincipalAxisNode::
set_max_abs_value( double _v)
{
  if( auto_range_)
    std::cerr << "Warning: Auto update min/max abs_values is enabled! Setting has no effect.\n";

  max_abs_value_ = _v;
}

//----------------------------------------------------------------------------


void 
PrincipalAxisNode::
set_min_draw_radius( double _v)
{
  min_draw_radius_ = _v;
  default_radius_ = false;
}


//----------------------------------------------------------------------------


void 
PrincipalAxisNode::
set_max_draw_radius( double _v)
{
  max_draw_radius_ = _v;
  default_radius_ = false;
}


//----------------------------------------------------------------------------


void 
PrincipalAxisNode::
auto_update_range()
{
  min_abs_value_ = std::numeric_limits<double>::max();
  max_abs_value_ = 0.0;

  for(unsigned int i=0; i<pc_.size(); ++i)
    if(draw_pc_[i])
      for(unsigned j=0; j<3; ++j)
	if(show_tensor_component_[j])
	{
	  max_abs_value_ = std::max( max_abs_value_, pc_[i].a[j].norm() );
	  min_abs_value_ = std::min( min_abs_value_, pc_[i].a[j].norm() );
	}

  update_bounding_box();
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
update_bounding_box()
{
  bool uninitialized = true;
  
  bbMin_ = Vec3d( FLT_MAX, FLT_MAX, FLT_MAX);
  bbMax_ = Vec3d(-FLT_MAX,-FLT_MAX,-FLT_MAX);

  for(unsigned int i=0; i<pc_.size(); ++i)
  {
    Vec3d lmin = (pc_[i].p) - Vec3d(1,1,1)*max_draw_radius_;
    Vec3d lmax = (pc_[i].p) + Vec3d(1,1,1)*max_draw_radius_;

    if(draw_pc_[i])
    {
      if(uninitialized)
      {
	bbMin_ = lmin;
	bbMax_ = lmax;
	uninitialized = false;
      }
      else
      {
	bbMin_.minimize(lmin);
	bbMax_.maximize(lmax);
      }
    }

  }
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
boundingBox(Vec3d& _bbMin, Vec3d& _bbMax)
{
  if( bbMin_ != Vec3d(FLT_MAX, FLT_MAX, FLT_MAX) )
  {
    _bbMin.minimize( bbMin_);
    _bbMax.maximize( bbMax_);
  }
}


//----------------------------------------------------------------------------

  
DrawModes::DrawMode 
PrincipalAxisNode::
availableDrawModes() const
{
  return ( DrawModes::POINTS              |
	   DrawModes::WIREFRAME           |
	   DrawModes::HIDDENLINE          |
	   DrawModes::SOLID_FLAT_SHADED   |
	   DrawModes::SOLID_SMOOTH_SHADED );
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
draw(GLState& /*_state*/, const DrawModes::DrawMode& _drawMode)
{
  if( draw_style_ == DS_3D)
    ACG::GLState::enable(GL_LIGHTING);
  else
    ACG::GLState::disable(GL_LIGHTING);

  if (_drawMode & DrawModes::SOLID_SMOOTH_SHADED)
    ACG::GLState::shadeModel(GL_SMOOTH);
  else
    ACG::GLState::shadeModel(GL_FLAT);

  glMatrixMode(GL_MODELVIEW);

  for(unsigned int i=0; i<pc_.size(); ++i)
    if( draw_pc_[i] )
       draw_principal_component( pc_[i]);
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
draw_principal_component(const PrincipalComponent& _pc)
{
  glPushMatrix();
  glTranslated( _pc.p[0], _pc.p[1], _pc.p[2]);

  //  for(unsigned int i=0; i<1; ++i)
  for(unsigned int i=0; i<3; ++i)
  {
    if( ! show_tensor_component_[i]) continue;

    Vec3d a;
    a  = _pc.a[i];

    // compute arrow length in world coords
    double length;
    length = std::max( min_abs_value_, a.norm());
    length = std::min( max_abs_value_, length  );

    if( a.norm() > 1e-8 )
      a.normalize();

    // Bug fixed: Visualizing all unit vectors yieled scaled_length = nan
    double scaled_length(min_draw_radius_);
    if( fabs(max_abs_value_-min_abs_value_) > 1e-6) 
      scaled_length += (length-min_abs_value_)/(max_abs_value_-min_abs_value_)*(max_draw_radius_-min_draw_radius_);

    a *= scaled_length;

//     std::cerr << "**************************\n";
//     std::cerr << "min_abs_value: " << min_abs_value_ << std::endl;
//     std::cerr << "max_abs_value: " << max_abs_value_ << std::endl;
//     std::cerr << "min_draw_radius: " << min_draw_radius_ << std::endl;
//     std::cerr << "max_draw_radius: " << max_draw_radius_ << std::endl;
//     std::cerr << "scaled_length: " << scaled_length << std::endl;
//     std::cerr << "length       : " << length << std::endl;
//     std::cerr << "sign         : " << _pc.sign[i] << std::endl;
//     std::cerr << "**************************\n";


    if( color_mode_ == CM_Sign)
    {
      // choose color due to eigenvalue sign
      if(_pc.sign[i] == true)
      {
        glColor3f(1.f, 0.f, 0.f);

        GLfloat mat_amb_diff[] = {1.0, 0.0, 0.0, 1.0};
        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
        glColor4fv( mat_amb_diff);
      }
      else
      {
        glColor3f(0.f, 0.f, 1.f);

        GLfloat mat_amb_diff[] = {0.0, 0.0, 1.0, 1.0};
        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
        glColor4fv( mat_amb_diff);
      }
    }
    else
      // also default
    {
      // choose color due to eigenvalue
      glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, axes_colors[i]);
      glColor4fv( axes_colors[i]);
    }

    // draw both axis
    if( draw_style_ == DS_3D)
    {
      double radius = max_draw_radius_ * 0.015 * cylinder_radius_scale_;

      draw_arrow( a     , radius);

      if( show_tensor_component_[i] == 2)
      draw_arrow( a*(-1), radius);
    }
    else
    {
      double width = cylinder_radius_scale_;

      draw_line( a     , width);
      if( show_tensor_component_[i] == 2)
        draw_line( a*(-1), width);
    }
  }

  glPopMatrix();
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
draw_arrow( const Vec3d& _axis, double _r)
{
  double base_radius = _r;
  double top_radius  = _r;
  double size        = _axis.norm();

  if( size > 1e-10)
  {
    glPushMatrix();

    unsigned int slices(8);

    Vec3d direction = _axis;
    Vec3d z_axis(0,0,1);
    Vec3d rot_normal;
    double rot_angle;

    direction.normalize();
    rot_angle  = acos((z_axis | direction))*180/M_PI;
    rot_normal = ((z_axis % direction).normalize());
  

    if(fabs(rot_angle) > 0.0001 && fabs(180-rot_angle) > 0.0001)
      glRotatef(rot_angle,rot_normal[0], rot_normal[1], rot_normal[2]);
    else
      glRotatef(rot_angle,1,0,0);

    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);

    gluCylinder(qobj, base_radius, top_radius, 0.85f*size, slices, 1);
    glRotatef( 180, 1, 0 ,0);
    gluDisk(qobj, 0, base_radius, slices, 1);
    glRotatef( 180, 1, 0 ,0);
    glTranslatef(0,0,0.85f*size);
    gluCylinder(qobj, 2.0*base_radius, 0, /*0.15f*size*/ 2.5*base_radius, slices, 1);
    glRotatef( 180, 1, 0 ,0);
    gluDisk(qobj, 0, 2.0*base_radius, slices, 1);


    glPopMatrix();
  }
}


//----------------------------------------------------------------------------


void
PrincipalAxisNode::
draw_line( const Vec3d& _axis, double _w)
{
  glLineWidth(_w);

  glBegin(GL_LINES);
  glVertex3f(0,0,0);
  glVertex3f(_axis[0], _axis[1], _axis[2]);
  glEnd();
}

//----------------------------------------------------------------------------


void
PrincipalAxisNode::
pick(GLState& /*_state*/, PickTarget _target)
{
  switch (_target)
  {
    case PICK_ANYTHING:
    case PICK_FACE: 
    { 
//       ACG::GLState::disable(GL_LIGHTING);
//       ACG::GLState::shadeModel(GL_FLAT);
//       glPushMatrix();
//       glTranslatef(position_[0], position_[1], position_[2]);
//       draw_obj();
//       glPopMatrix();
      break; 
    }

    default:
      break;
  }      
}


//----------------------------------------------------------------------------


void PrincipalAxisNode::set_axes_colors(const Vec4f colors[3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            axes_colors[i][j] = colors[i][j];
        }
    }
}

void PrincipalAxisNode::get_axes_colors(Vec4f out_colors[3]) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            out_colors[i][j] = axes_colors[i][j];
        }
    }
}


//----------------------------------------------------------------------------


// Following code is taken from http://stackoverflow.com/questions/4372224/fast-method-for-computing-3x3-symmetric-matrix-spectral-decomposition:
// Slightly modified version of  Stan Melax's code for 3x3 matrix diagonalization (Thanks Stan!)
// source: http://www.melax.com/diag.html?attredirects=0
void PrincipalAxisNode::diagonalize(const double (&A)[3][3], double (&Q)[3][3], double (&D)[3][3])
{
    // A must be a symmetric matrix.
    // returns Q and D such that
    // Diagonal matrix D = QT * A * Q;  and  A = Q*D*QT
    const int maxsteps=24;  // certainly wont need that many.
    int k0, k1, k2;
    double o[3], m[3];
    double q [4] = {0.0,0.0,0.0,1.0};
    double jr[4];
    double sqw, sqx, sqy, sqz;
    double tmp1, tmp2, mq;
    double AQ[3][3];
    double thet, sgn, t, c;
    for(int i=0;i < maxsteps;++i)
    {
        // quat to matrix
        sqx      = q[0]*q[0];
        sqy      = q[1]*q[1];
        sqz      = q[2]*q[2];
        sqw      = q[3]*q[3];
        Q[0][0]  = ( sqx - sqy - sqz + sqw);
        Q[1][1]  = (-sqx + sqy - sqz + sqw);
        Q[2][2]  = (-sqx - sqy + sqz + sqw);
        tmp1     = q[0]*q[1];
        tmp2     = q[2]*q[3];
        Q[1][0]  = 2.0 * (tmp1 + tmp2);
        Q[0][1]  = 2.0 * (tmp1 - tmp2);
        tmp1     = q[0]*q[2];
        tmp2     = q[1]*q[3];
        Q[2][0]  = 2.0 * (tmp1 - tmp2);
        Q[0][2]  = 2.0 * (tmp1 + tmp2);
        tmp1     = q[1]*q[2];
        tmp2     = q[0]*q[3];
        Q[2][1]  = 2.0 * (tmp1 + tmp2);
        Q[1][2]  = 2.0 * (tmp1 - tmp2);

        // AQ = A * Q
        AQ[0][0] = Q[0][0]*A[0][0]+Q[1][0]*A[0][1]+Q[2][0]*A[0][2];
        AQ[0][1] = Q[0][1]*A[0][0]+Q[1][1]*A[0][1]+Q[2][1]*A[0][2];
        AQ[0][2] = Q[0][2]*A[0][0]+Q[1][2]*A[0][1]+Q[2][2]*A[0][2];
        AQ[1][0] = Q[0][0]*A[0][1]+Q[1][0]*A[1][1]+Q[2][0]*A[1][2];
        AQ[1][1] = Q[0][1]*A[0][1]+Q[1][1]*A[1][1]+Q[2][1]*A[1][2];
        AQ[1][2] = Q[0][2]*A[0][1]+Q[1][2]*A[1][1]+Q[2][2]*A[1][2];
        AQ[2][0] = Q[0][0]*A[0][2]+Q[1][0]*A[1][2]+Q[2][0]*A[2][2];
        AQ[2][1] = Q[0][1]*A[0][2]+Q[1][1]*A[1][2]+Q[2][1]*A[2][2];
        AQ[2][2] = Q[0][2]*A[0][2]+Q[1][2]*A[1][2]+Q[2][2]*A[2][2];
        // D = Qt * AQ
        D[0][0] = AQ[0][0]*Q[0][0]+AQ[1][0]*Q[1][0]+AQ[2][0]*Q[2][0];
        D[0][1] = AQ[0][0]*Q[0][1]+AQ[1][0]*Q[1][1]+AQ[2][0]*Q[2][1];
        D[0][2] = AQ[0][0]*Q[0][2]+AQ[1][0]*Q[1][2]+AQ[2][0]*Q[2][2];
        D[1][0] = AQ[0][1]*Q[0][0]+AQ[1][1]*Q[1][0]+AQ[2][1]*Q[2][0];
        D[1][1] = AQ[0][1]*Q[0][1]+AQ[1][1]*Q[1][1]+AQ[2][1]*Q[2][1];
        D[1][2] = AQ[0][1]*Q[0][2]+AQ[1][1]*Q[1][2]+AQ[2][1]*Q[2][2];
        D[2][0] = AQ[0][2]*Q[0][0]+AQ[1][2]*Q[1][0]+AQ[2][2]*Q[2][0];
        D[2][1] = AQ[0][2]*Q[0][1]+AQ[1][2]*Q[1][1]+AQ[2][2]*Q[2][1];
        D[2][2] = AQ[0][2]*Q[0][2]+AQ[1][2]*Q[1][2]+AQ[2][2]*Q[2][2];
        o[0]    = D[1][2];
        o[1]    = D[0][2];
        o[2]    = D[0][1];
        m[0]    = fabs(o[0]);
        m[1]    = fabs(o[1]);
        m[2]    = fabs(o[2]);

        k0      = (m[0] > m[1] && m[0] > m[2])?0: (m[1] > m[2])? 1 : 2; // index of largest element of offdiag
        k1      = (k0+1)%3;
        k2      = (k0+2)%3;
        if (o[k0]==0.0)
        {
            break;  // diagonal already
        }
        thet    = (D[k2][k2]-D[k1][k1])/(2.0*o[k0]);
        sgn     = (thet > 0.0)?1.0:-1.0;
        thet   *= sgn; // make it positive
        t       = sgn /(thet +((thet < 1.E6)?sqrt(thet*thet+1.0):thet)) ; // sign(T)/(|T|+sqrt(T^2+1))
        c       = 1.0/sqrt(t*t+1.0); //  c= 1/(t^2+1) , t=s/c
        if(c==1.0)
        {
            break;  // no room for improvement - reached machine precision.
        }
        jr[0 ]  = jr[1] = jr[2] = jr[3] = 0.0;
        jr[k0]  = sgn*sqrt((1.0-c)/2.0);  // using 1/2 angle identity sin(a/2) = sqrt((1-cos(a))/2)
        jr[k0] *= -1.0; // since our quat-to-matrix convention was for v*M instead of M*v
        jr[3 ]  = sqrt(1.0f - jr[k0] * jr[k0]);
        if(jr[3]==1.0)
        {
            break; // reached limits of floating point precision
        }
        q[0]    = (q[3]*jr[0] + q[0]*jr[3] + q[1]*jr[2] - q[2]*jr[1]);
        q[1]    = (q[3]*jr[1] - q[0]*jr[2] + q[1]*jr[3] + q[2]*jr[0]);
        q[2]    = (q[3]*jr[2] + q[0]*jr[1] - q[1]*jr[0] + q[2]*jr[3]);
        q[3]    = (q[3]*jr[3] - q[0]*jr[0] - q[1]*jr[1] - q[2]*jr[2]);
        mq      = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
        q[0]   /= mq;
        q[1]   /= mq;
        q[2]   /= mq;
        q[3]   /= mq;
    }
}


//=============================================================================
} // namespace SceneGraph
} // namespace ACG
//=============================================================================
