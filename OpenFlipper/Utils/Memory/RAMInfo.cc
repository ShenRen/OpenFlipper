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

// Main Memory information
#ifdef WIN32
#include <Windows.h>
#elif defined ARCH_DARWIN
#else
#include <sys/sysinfo.h>
//Info class returned by sysinfo
//struct sysinfo {
//  long uptime;              /* Sekunden seit dem letzten Systemstart */
//  unsigned long loads[3];   /* 1, 5 und 15 minütige Systemlast */
//  unsigned long totalram;   /* nutzbare Hauptspeichergröße */
//  unsigned long freeram;    /* verfügbare Speichergröße */
//  unsigned long sharedram;  /* Größe des gemeinsamen Speichers */
//  unsigned long bufferram;  /* von Puffern benutzter Speicher */
//  unsigned long totalswap;  /* Größe des Auslagerungsspeichers */
//  unsigned long freeswap;   /* verfügbarer Auslagerungsspeicher */
//  unsigned short procs;     /* Aktuelle Prozesszahl */
//  unsigned long totalhigh;  /* Gesamtgröße des oberen Speicherbereichs */
//  unsigned long freehigh;   /* verfügbarer oberer Speicherbereich */
//  unsigned int mem_unit;    /* Größe der Speichereinheit in Byte */
//  char _f[20-2*sizeof(long)-sizeof(int)]; /* Auffüllung auf 64 bytes */
//};

// private struct to get ram information
#endif
namespace{
struct MemoryVacancy{
  unsigned long totalRamMB;
  unsigned long freeRamMB;
  unsigned long bufferRamMB;
};

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

      struct sysinfo sys_info;
      sysinfo(&sys_info);

      // Unit in bytes ; /1024 -> KB ; /1024 MB
      _outMemoryVacancy.totalRamMB = sys_info.totalram / 1024 / 1024 * sys_info.mem_unit;
      _outMemoryVacancy.freeRamMB = sys_info.freeram / 1024 / 1024 * sys_info.mem_unit;
      _outMemoryVacancy.bufferRamMB = sys_info.bufferram / 1024 / 1024 * sys_info.mem_unit; // Buffers get freed, if we don't have enough free ram
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
