
#ifndef RAMINFO_HH
#define RAMINFO_HH

#include "OpenFlipper/common/GlobalDefines.hh"

namespace Utils
{
  namespace Memory {

    /**
     * @brief The MemoryVacancy struct holds information about vacant buffered and total memory
     */
    struct DLLEXPORT MemoryVacancy{
      unsigned long totalRamMB;
      unsigned long freeRamMB;
      unsigned long bufferRamMB;
    };

    /**
     * @brief MemoryInfoUpdate queries information about the RAM and writes them to a MemoryVacancy struct
     * @param _outMemoryVacancy the Struct that shall hold the queried information.
     */
    DLLEXPORT
    void MemoryInfoUpdate(MemoryVacancy& _outMemoryVacancy);

  }
}

#endif // RAMINFO_HH
