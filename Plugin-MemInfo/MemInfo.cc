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



#include "MemInfo.hh"
#include <ACG/GL/gl.hh>
#include <QtGlobal>
#include <OpenFlipper/Utils/Memory/RAMInfo.hh>

// GPU Memory information
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX   0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049

void MemInfoPlugin::initializePlugin()
{

}


void MemInfoPlugin::pluginsInitialized() {

  // Check extension for NVIDIA memory information
  QString glExtensions = QString((const char*)glGetString(GL_EXTENSIONS));

  updateTimer_ = new QTimer();
  updateTimer_->setSingleShot(false);

  // NVIDIA cards
  if ( glExtensions.contains("GL_NVX_gpu_memory_info") ) {

//    emit log(LOGINFO,"NVIDIA card Memory info supported, installing gpu memory monitor into status bar");

    gpuMemBar_= new QProgressBar();
    gpuMemBar_->setFixedWidth(260);
    gpuMemBar_->setFormat( "GPU Mem %p% %v/%m MB" );

    emit addWidgetToStatusbar(gpuMemBar_);

    connect(updateTimer_,SIGNAL(timeout()),this,SLOT(nvidiaMemoryInfoUpdate()));

    updateTimer_->start(4000);

    nvidiaMemoryInfoUpdate();
  }

  // Main Memory information
  #ifdef ARCH_DARWIN //Apple

  #else // Linux and Windows

 //   emit log(LOGINFO,"Main Memory monitoring supported, installing main memory monitor into status bar");

    mainMemBar_= new QProgressBar();
    mainMemBar_->setFixedWidth(260);
    mainMemBar_->setFormat( "Mem %p% %v/%m MB" );

    emit addWidgetToStatusbar(mainMemBar_);

    connect(updateTimer_,SIGNAL(timeout()),this,SLOT(cpuMemoryInfoUpdate()));

    updateTimer_->start(4000);

    cpuMemoryInfoUpdate();

  #endif

}

MemInfoPlugin::MemInfoPlugin():
    gpuMemBar_(NULL),
    mainMemBar_(NULL),
    updateTimer_(NULL)
{

}

MemInfoPlugin::~MemInfoPlugin() {
  delete updateTimer_;
}

void MemInfoPlugin::nvidiaMemoryInfoUpdate() {

  if (gpuMemBar_) {
    // get total memory on gpu
    GLint total_mem_kb = 0;
    glGetIntegerv(GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &total_mem_kb);

    // get currently available memory on gpu
    GLint cur_avail_mem_kb = 0;
    glGetIntegerv(GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &cur_avail_mem_kb);

    gpuMemBar_->setRange(  0 , total_mem_kb/1024 );
    gpuMemBar_->setValue( (total_mem_kb-cur_avail_mem_kb)/1024);
    setProgressBarStyleSheet(gpuMemBar_);
  }
}

void MemInfoPlugin::cpuMemoryInfoUpdate() {

  if (mainMemBar_) {
    unsigned long totalRamMB = Utils::Memory::queryTotalRAM();
    unsigned long freeRamMB = Utils::Memory::queryFreeRAM();
    unsigned long bufferRamMB = Utils::Memory::queryBufferedRAM();

    mainMemBar_->setRange(  0 , totalRamMB  );
    mainMemBar_->setValue( totalRamMB-freeRamMB-bufferRamMB);
    setProgressBarStyleSheet(mainMemBar_);
  }
}


void MemInfoPlugin::setProgressBarStyleSheet(QProgressBar* _bar)
{
  const int val = _bar->value();
  const int maxVal = _bar->maximum();
  
  // red starts with 50% mem alloc with 0 and is 1 at 100% mem alloc
  float redPerc = (val > 0.5f*maxVal) ? 2.f*(val-0.5f*maxVal) : 0.f;
  const quint32 red = 255.f*redPerc/maxVal;
  const quint32 green = 255u-red;
  const quint32 blue = 0u;

  //save color in a 32bit integer
  const quint32 color = (red<<16)+(green<<8)+blue;
  _bar->setStyleSheet(QString(" QProgressBar { border: 2px solid grey; border-radius: 2px; text-align: center; } QProgressBar::chunk {background-color: #%1; width: 1px;}").arg(color,6,16,QChar('0')));
  
}

#if QT_VERSION < 0x050000
  Q_EXPORT_PLUGIN2( meminfoplugin , MemInfoPlugin );
#endif

