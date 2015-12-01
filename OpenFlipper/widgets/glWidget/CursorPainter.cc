/*===========================================================================*\
*                                                                            *
*                              OpenFlipper                                   *
 *           Copyright (c) 2001-2015, RWTH-Aachen University                 *
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
*                                                                            *
\*===========================================================================*/

/*===========================================================================*\
*                                                                            *
*   $Revision$                                                       *
*   $LastChangedBy$                                                *
*   $Date$                     *
*                                                                            *
\*===========================================================================*/




//=============================================================================
//
//  CLASS CursorPainter - IMPLEMENTATION
//
//=============================================================================

//== INCLUDES =================================================================

#include <QPixmap>
#include <QBitmap>

#include <OpenFlipper/common/GlobalOptions.hh>

#include "CursorPainter.hh"
#include "QtBaseViewer.hh"

//== NAMESPACES ===============================================================

CursorPainter::CursorPainter (QObject *_parent) :
  QObject(_parent),
  cursor_(),
  initialized_(false),
  enabled_(false),
  mouseIn_(false),
  forceNative_(false),
  xOff_(0),
  yOff_(0),
  texture_(0),
  hasCursor_(false)
{
}

//-----------------------------------------------------------------------------

CursorPainter::~CursorPainter ()
{
  if (initialized_)
  {
    glDeleteTextures (1, &texture_);
  }
}

//-----------------------------------------------------------------------------

void CursorPainter::setCursor (const QCursor &_cursor)
{
  nativeCursor_ = _cursor;
  cursorToCursor ();
  cursorToTexture ();
  if (!(initialized_ && enabled_ && hasCursor_) || forceNative_) {
    foreach (glViewer *v, views_)
      v->setCursor ((forceNative_)? nativeCursor_ : cursor_);
  }
}

//-----------------------------------------------------------------------------

void CursorPainter::initializeGL()
{
  if (initialized_)
    return;
  initialized_ = true;

  // setup cursor texture
  glGenTextures (1, &texture_);

  ACG::GLState::bindTexture (GL_TEXTURE_2D, texture_);
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  ACG::GLState::bindTexture (GL_TEXTURE_2D, 0);

  cursorToTexture ();

  if (enabled_ && hasCursor_ && !forceNative_)
  {
    foreach (glViewer *v, views_)
      v->setCursor (Qt::BlankCursor);
    
  }
  else
  {
    foreach (glViewer *v, views_)
      v->setCursor ((forceNative_)? nativeCursor_ : cursor_);
    
  }
}

//-----------------------------------------------------------------------------

void CursorPainter::registerViewer(glViewer * _viewer)
{
  views_.append (_viewer);
  _viewer->setCursorPainter (this);
}

//-----------------------------------------------------------------------------

void CursorPainter::paintCursor (ACG::GLState *_state)
{
  if (!initialized_)
    return;

  if (!enabled())
    return;

  // project (0, 0, 0) to get its position on the screen
  ACG::Vec3d zPos = _state->project (ACG::Vec3d (0.0, 0.0, 0.0));
  // unproject the result translated by 1px in x and y
  zPos = _state->unproject (ACG::Vec3d (zPos[0] + 1, zPos[1] + 1, zPos[2]));

  // this gives us the size of one pixel in the current scene transformation
  // now we can paint the cursor always with the same width/height
  float xscale = zPos[0];
  float yscale = -zPos[1];

  glPushAttrib (GL_ALL_ATTRIB_BITS);

  ACG::GLState::disable (GL_DEPTH_TEST);
  ACG::GLState::disable(GL_LIGHTING);
  ACG::GLState::enable(GL_BLEND);

  ACG::GLState::blendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // bind texture
  ACG::GLState::enable (GL_TEXTURE_2D);
  ACG::GLState::bindTexture (GL_TEXTURE_2D, texture_);

  glColor4f (1.0, 1.0, 1.0, 1.0);

  float x1 = -xOff_ * xscale;
  float x2 = (32 - xOff_) * xscale;
  float y1 = -yOff_ * yscale;
  float y2 = (32 - yOff_) * yscale;

  // draw cursor quad
  glBegin (GL_QUADS);
  glTexCoord2f (0, 0);
  glVertex3f (x1, y1, 0);
  glTexCoord2f (0, 1);
  glVertex3f (x1, y2, 0);
  glTexCoord2f (1, 1);
  glVertex3f (x2, y2, 0);
  glTexCoord2f (1, 0);
  glVertex3f (x2, y1, 0);
  glEnd ();


  glPopAttrib ();
}

//-----------------------------------------------------------------------------

void CursorPainter::updateCursorPosition (QPointF _scenePos)
{
  cursorPos_ = _scenePos;
  setMouseIn (true);
}

//-----------------------------------------------------------------------------

QPointF CursorPainter::cursorPosition()
{
  return cursorPos_;
}

//-----------------------------------------------------------------------------

void CursorPainter::setEnabled(bool _enabled)
{
  enabled_ = _enabled;

  if (initialized_)
  {
    if (_enabled && hasCursor_)
    {
      foreach (glViewer *v, views_)
        v->setCursor (Qt::BlankCursor);
    }
    else
    {
      foreach (glViewer *v, views_)
        v->setCursor ((forceNative_)? nativeCursor_ : cursor_);
    }
  } 
}

//-----------------------------------------------------------------------------

bool CursorPainter::enabled()
{
  return initialized_ && enabled_ && hasCursor_ && mouseIn_ && !forceNative_;
}

//-----------------------------------------------------------------------------

void CursorPainter::cursorToTexture()
{
  
  if (!initialized_) {
    return;
  }

  unsigned char buf[4096];
  QImage cImg;

  hasCursor_ = false;

  // Initialize hotspot with default position in the upper left corner of the cursor
  xOff_ = 0;
  yOff_ = 0;
  
  ///\todo Possibly switch all cursors to bitmap cursors and check there Hotspots!!
  switch (nativeCursor_.shape())
  {
    case Qt::ArrowCursor:
      cImg.load (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"cursor_arrow.png");
      break;
    case Qt::PointingHandCursor:
      cImg.load (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"cursor_move.png");
      break;
    case Qt::WhatsThisCursor:       
      cImg.load (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"cursor_whatsthis.png");
      break;      
    case Qt::BitmapCursor: 
      // Get the image of the cursor
      cImg = QImage(( nativeCursor_.pixmap().toImage() ) );
      
      
      // get the hotspot from the cursor
      xOff_ = nativeCursor_.hotSpot().x();
      yOff_ = nativeCursor_.hotSpot().y();
      break;
    default: 
      std::cerr << "cursorToTexture: Unknown cursor shape!" << nativeCursor_.shape() << std::endl;
      return;
  }
  
  // Check if the cursor dimension is matching our requirements
  if (cImg.width () != 32 || cImg.height () != 32) {
    std::cerr << "cursorToTexture: Dimension error" << nativeCursor_.shape() << std::endl;
    return;
  }

  // convert ARGB QImage to RGBA for gl
  int index = 0;
  for (int y = 0; y < cImg.height (); y++)
    for (int x = 0; x < cImg.width (); x++)
      {
        QRgb pix = cImg.pixel (x, y);
        buf[index] = qRed (pix);
        buf[index+1] = qGreen (pix);
        buf[index+2] = qBlue (pix);
        buf[index+3] = qAlpha (pix);
        index += 4;
      }

  ACG::GLState::enable (GL_TEXTURE_2D);
  ACG::GLState::bindTexture (GL_TEXTURE_2D, texture_);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, buf);
  ACG::GLState::bindTexture (GL_TEXTURE_2D, 0);
  ACG::GLState::disable (GL_TEXTURE_2D);

  hasCursor_ = true;
}

//-----------------------------------------------------------------------------

void CursorPainter::setMouseIn(bool _in)
{
  mouseIn_ = _in;
}

//-----------------------------------------------------------------------------

QRectF CursorPainter::cursorBoundingBox()
{
  return QRectF (-xOff_, -yOff_, 32, 32);
}

//=============================================================================
//=============================================================================

void CursorPainter::setForceNative(bool _enabled)
{
  forceNative_ = _enabled;
  
  if (!(initialized_ && enabled_ && hasCursor_) || forceNative_)
  {
    foreach (glViewer *v, views_)
      v->setCursor ((forceNative_)? nativeCursor_ : cursor_);
    
  }
  else
  {
    foreach (glViewer *v, views_)
        v->setCursor (Qt::BlankCursor);
  }
}

void CursorPainter::cursorToCursor()
{
  QPixmap pix;

  switch (nativeCursor_.shape())
  {
    case Qt::ArrowCursor:
      pix.load (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"cursor_arrow.png");
      if (!pix.isNull() && pix.width() == 32 && pix.height() == 32)
      {
        cursor_ = QCursor (pix, 0, 0);
      }
      else
        cursor_ = nativeCursor_;
      break;
    case Qt::PointingHandCursor:
      pix.load (OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"cursor_move.png");
      if (!pix.isNull() && pix.width() == 32 && pix.height() == 32)
      {
        cursor_ = QCursor (pix, 0, 0);
      }
      else
        cursor_ = nativeCursor_;
      break;
    default:
      cursor_ = nativeCursor_;
      return;
  }
}



