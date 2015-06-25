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
#endif

#if PROCPS_ENABLED
#include "procps/sysinfo.h"
#endif

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

    #elif defined ARCH_DARWIN // Apple
    #elif PROCPS_ENABLED
      meminfo();
      _outMemoryVacancy.totalRamMB = kb_main_total / 1024;
      _outMemoryVacancy.freeRamMB = kb_main_free / 1024;
      _outMemoryVacancy.bufferRamMB = (kb_main_buffers + kb_main_cached) / 1024;
    #else // Linux

      struct sysinfo sys_info;
      sysinfo(&sys_info);

      // Unit in bytes ; /1024 -> KB ; /1024 MB
      _outMemoryVacancy.totalRamMB = sys_info.totalram / 1024 / 1024 * sys_info.mem_unit;
      _outMemoryVacancy.freeRamMB = sys_info.freeram / 1024 / 1024 * sys_info.mem_unit;
      _outMemoryVacancy.bufferRamMB = sys_info.bufferram / 1024 / 1024 * sys_info.mem_unit; // Buffers get freed, if we don't have enough free ram
    #endif
    }
  }
}
