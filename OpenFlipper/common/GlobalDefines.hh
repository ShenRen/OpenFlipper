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
//  Global defines for OpenFlipper
//
//=============================================================================

/**
 * \file GlobalDefines.hh
 * This File contains the required defines for the OpenFlipper Framework
 */

#ifndef GLOBALDEFINES_HH
#define GLOBALDEFINES_HH


/**
 * Get GCC version and convert to MMmm where MM is the major
 * and mm the minor release version.
 * For example if GCC_VERSION == 4030, we consider gcc-4.3.
 */

#define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__ * 10)

//== DEEFINES =================================================================

/** Force static casts for Properties in OpenMesh.
 * Cross Plugin dynamic casts are not possible due to locally defined symbols
 */
#define OM_FORCE_STATIC_CAST

/** Enable openmp where possible (for gcc version >= 4.3)
 */
#ifndef USE_OPENMP
  #if GCC_VERSION >= 4030
    #define USE_OPENMP
  #endif
#endif


#ifndef DLLEXPORT
	#ifdef WIN32
		#ifdef PLUGINLIBDLL
			#ifdef USEPLUGINLIBDLL
				#define DLLEXPORT __declspec(dllimport)
				#define DLLEXPORTONLY
			#else
				#define DLLEXPORT __declspec(dllexport)
				#define DLLEXPORTONLY __declspec(dllexport)
			#endif
		#else
			#define DLLEXPORT
			#define DLLEXPORTONLY
		#endif
	#else
		#define DLLEXPORT
		#define DLLEXPORTONLY
	#endif
#endif

// Disable the warnings about needs to have DLL interface as we have tons of vector templates
#ifdef WIN32
  #pragma warning( disable: 4251 )
#endif


//=============================================================================
#endif // GLOBALDEFINES_HH defined
//=============================================================================
