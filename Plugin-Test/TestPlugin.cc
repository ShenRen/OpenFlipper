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



#include "TestPlugin.hh"
#include <ACG/GL/ShaderCache.hh>








void TestPlugin::initializePlugin()
{
}

void TestPlugin::pluginsInitialized() {
    
  QWidget* tool_ = new QWidget();
  QSize size(100, 100);
  tool_->resize(size);

  QVBoxLayout* layout = new QVBoxLayout(tool_);

  QPushButton* button = new QPushButton("click", tool_);
  
  layout->addWidget(button);

  // connect signals->slots
  connect(button, SIGNAL(clicked()), this, SLOT(btnclick()));


  emit addToolbox(tr("Test"), tool_);

}




void TestPlugin::btnclick()
{
  const char* filename = "c:/dbg/attrinfo/1.glsl";

//   const ACG::ShaderCache::VertexShaderAttributeInfoVec& attr =
//     ACG::ShaderCache::getInstance()->getVertexShaderAttributeInfo(filename);
// 
//   std::cout << "attributes found in :" << filename << std::endl;
//   for (int i = 0; i < attr.size(); ++i)
//     std::cout << i << ": " << attr[i].name << " -  size: " << attr[i].size << std::endl;

}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( testplugin , TestPlugin );
#endif

