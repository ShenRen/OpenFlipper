/*===========================================================================*\
*                                                                            *
 *                              OpenFlipper                                   *
 *      Copyright (C) 2001-2014 by Computer Graphics Group, RWTH Aachen       *
 *                           www.openflipper.org                              *
 *                                                                            *
 *--------------------------------------------------------------------------- *
 *  This file is part of OpenFlipper.                                         *
 *                                                                            *
 *  OpenFlipper is free software: you can redistribute it and/or modify       *
 *  it under the terms of the GNU Lesser General Public License as            *
 *  published by the Free Software Foundation, either version 3 of            *
 *  the License, or (at your option) any later version with the               *
 *  following exceptions:                                                     *
 *                                                                            *
 *  If other files instantiate templates or use macros                        *
 *  or inline functions from this file, or you compile this file and          *
 *  link it with other files to produce an executable, this file does         *
 *  not by itself cause the resulting executable to be covered by the         *
 *  GNU Lesser General Public License. This exception does not however        *
 *  invalidate any other reasons why the executable file might be             *
 *  covered by the GNU Lesser General Public License.                         *
 *                                                                            *
 *  OpenFlipper is distributed in the hope that it will be useful,            *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU Lesser General Public License for more details.                       *
 *                                                                            *
 *  You should have received a copy of the GNU LesserGeneral Public           *
 *  License along with OpenFlipper. If not,                                   *
 *  see <http://www.gnu.org/licenses/>.                                       *
 *                                                                            *
 \*===========================================================================*/

#include "RAMInfo.hh"

// Main Memory information
#ifdef WIN32
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
