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

#include "RAMInfo.hh"
#include <stdio.h>

// Main Memory information
#ifdef WIN32
#include <Windows.h>
#elif defined ARCH_DARWIN
#endif
// private struct to get ram information
namespace{
struct MemoryVacancy{
  unsigned long totalRamMB;
  unsigned long freeRamMB;
  unsigned long bufferRamMB;
};

void parseMeminfo(int& total, int& free, int& buffer)
{
  int memcache;
  int memfree;
  FILE* info = fopen("/proc/meminfo","r");
  if(fscanf (info, "MemTotal: %d kB MemFree: %d kB Buffers: %d kB Cached: %d kB",&total, &memfree, &buffer, &memcache) < 4) //try to parse the old meminfo format
  {
    fclose(info);
    info = fopen("/proc/meminfo","r");
    //parsing the old format failed so we try to parse using the new format
    if(fscanf(info, "MemTotal: %d kB MemFree: %d kB MemAvailable: %d kB Buffers: %d kB Cached: %d kB",&total, &memfree, &free, &buffer, &memcache) < 5)
    {
      //parsing failed overall so return -1 for all values
      total = -1;
      free = -1;
      buffer = -1;
    }
    else
    {
      free = memfree + (buffer + memcache);  //everything is fine
    }

  }
  else  //compute available memory
  {
    free = memfree + (buffer + memcache);
  }
  fclose(info);
}

}
namespace Utils
{
  namespace Memory
  {
    void MemoryInfoUpdate(MemoryVacancy & _outMemoryVacancy) {

      //initialize to 0 just in case something fails or cant be read
      _outMemoryVacancy.totalRamMB = 0;
      _outMemoryVacancy.freeRamMB = 0;
      _outMemoryVacancy.bufferRamMB = 0;

      // Main Memory information
    #ifdef WIN32 //Windows
      // Define memory structure
      MEMORYSTATUSEX ms;
      // Set the size ( required according to spec ... why???? )
      ms.dwLength = sizeof(ms);
      // Get the info
      GlobalMemoryStatusEx(&ms);

      _outMemoryVacancy.totalRamMB = ms.ullTotalPhys / 1024 / 1024;
      _outMemoryVacancy.freeRamMB = ms.ullAvailPhys / 1024 / 1024;

    #elif defined ARCH_DARWIN // Apple (sadly cant query free memory)
    #else // Linux

      int total, free, buffer;
      parseMeminfo(total, free, buffer);

      // Unit in bytes ; /1024 -> MB
      _outMemoryVacancy.totalRamMB = (long)total / 1024;
      _outMemoryVacancy.freeRamMB = (long)free / 1024;
      _outMemoryVacancy.bufferRamMB = (long)buffer / 1024; // Buffers get freed, if we don't have enough free ram
    #endif
    }

   unsigned long queryFreeRAM()
   {
     MemoryVacancy vac;
     MemoryInfoUpdate(vac);
     return vac.freeRamMB;
   }

   unsigned long queryTotalRAM()
   {
     MemoryVacancy vac;
     MemoryInfoUpdate(vac);
     return vac.totalRamMB;
   }

   unsigned long queryBufferedRAM()
   {
     MemoryVacancy vac;
     MemoryInfoUpdate(vac);
     return vac.bufferRamMB;
   }
  }
}
