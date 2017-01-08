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



#include "MeshConvert.hh"
#include <OpenFlipper/common/GlobalOptions.hh>
#include <OpenFlipper/BasePlugin/PluginFunctions.hh>
#include <ObjectTypes/PolyMesh/PolyMesh.hh>
#include <ObjectTypes/TriangleMesh/TriangleMesh.hh>
#include <ObjectTypes/PolyMesh/PluginFunctionsPolyMesh.hh>
#include <OpenFlipper/libs_required/OpenMesh/src/OpenMesh/Core/Mesh/Casts.hh>


void MeshConvertPlugin::initializePlugin()
{

}


void MeshConvertPlugin::pluginsInitialized()
{

  // Create your toolbar
  toolbar = new QToolBar(tr("Example Toolbar"));

  grp = new QActionGroup(toolbar);

  // Create an action for the toolbar
  bidirectionalConversion = new QAction(tr("&Convert Meshes"), grp);
  polyConversion = new QAction(tr("&Convert to PolyMesh"), grp);
  triConversion = new QAction(tr("&Convert to TriMesh"), grp);

  // Create an icon which is shown for the action
  bidirectionalConversion->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"Mesh-Convert.png"));
  polyConversion->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"Mesh-Convert-Poly.png"));
  triConversion->setIcon(QIcon(OpenFlipper::Options::iconDirStr()+OpenFlipper::Options::dirSeparator()+"Mesh-Convert-Tri.png"));

  // Add the action to the toolbar
  toolbar->addAction(bidirectionalConversion);
  toolbar->addAction(polyConversion);
  toolbar->addAction(triConversion);



  connect( grp, SIGNAL( triggered(QAction*) ), this, SLOT(convert(QAction*)) );

  // Integrate the new toolbar into OpenFlipper
  emit addToolbar( toolbar );

}

MeshConvertPlugin::MeshConvertPlugin()
{

}

MeshConvertPlugin::~MeshConvertPlugin()
{

}

void MeshConvertPlugin::convert(QAction* _action)
{
  std::vector<int> _ids;
  if(! PluginFunctions::getTargetIdentifiers( _ids  ))
    return;
  BaseObject* obj;
  PolyMesh* p;
  TriMesh* t;
  for(std::vector<int>::iterator id = _ids.begin(); id != _ids.end(); ++id)
  {
    if((_action == bidirectionalConversion || _action == polyConversion) &&
            PluginFunctions::getMesh(*id,t))
    {
      PolyMesh  converted = static_cast<PolyMesh>(*t);
      int newID = -1;
      emit addEmptyObject(DATA_POLY_MESH, newID);
      if(PluginFunctions::getMesh(newID,p))
      {
        *p = converted;
        emit updatedObject(newID);
      }
    }
    if((_action == bidirectionalConversion || _action == triConversion) &&
            PluginFunctions::getMesh(*id,p))
    {
      TriMesh  converted = static_cast<TriMesh>(*p);
      int newID = -1;
      emit addEmptyObject(DATA_TRIANGLE_MESH, newID);
      if(PluginFunctions::getMesh(newID,t))
      {
        *t = converted;
        emit updatedObject(newID);
      }
    }
  }
}


#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2( meshconvertplugin , MeshConvertPlugin );
#endif

