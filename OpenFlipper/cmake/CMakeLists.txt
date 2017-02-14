# Force minimal cmake version
# Required 3.1 to set C++11 
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

file(SHA512 ${CMAKE_SOURCE_DIR}/CMakeLists.txt HASH_TOPLEVEL_CMAKEFILE)
file(SHA512 ${CMAKE_SOURCE_DIR}/OpenFlipper/cmake/CMakeLists.txt HASH_LATEST_CMAKEFILE)

if ( NOT ${HASH_TOPLEVEL_CMAKEFILE} EQUAL ${HASH_LATEST_CMAKEFILE} ) 
  message(WARNING "WARNING! Possibly Outdated toplevel CMakeLists.txt. Please update the toplevel copy by the file OpenFlipper/cmake/CMakeLists.txt .")
endif()

# Convenience checks for problems
if(EXISTS ${CMAKE_SOURCE_DIR}/libs_required/OpenMesh)
  message(FATAL_ERROR "Directory libs_required/OpenMesh has been moved to OpenFlipper/libs_required/OpenMesh! Please remove the old directory if it's still there!")
endif()

# Set and enforce C++-11 flags
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )
set( CMAKE_CXX_STANDARD 11 )

# ACG Environment default settings
# This is ugly but currently we need to work around the default installed 5.3 on debian
if ( EXISTS /ACG/acgdev/gcc-4.9-x86_64/qt-5.6.0/5.6/gcc_64/ )
  # Default to this install path for QT%_INSTALL_DIR
  set (QT5_INSTALL_PATH /ACG/acgdev/gcc-4.9-x86_64/qt-5.6.0/5.6/gcc_64/ CACHE PATH "Qt5 install path set for ACG environment" )
endif()

if ( WIN32 )
  # This is the base directory for windows library search used in the finders we shipp.
  set(CMAKE_WINDOWS_LIBS_DIR "c:/libs" CACHE STRING "Default Library search dir on windows." )
endif()

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()


IF ( APPLE )
	IF(CMAKE_BUILD_TYPE MATCHES Debug)
	    message(WARNING "WARNING: Mac Debug mode might mix debug and release libraries (Webkit, ...), which causes strange errors. If you use ports to provide qt, you might want to disable Qt debug libraries to make sure only release qt libraries are linked to OpenFlippers debug build.")
	ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)
endif()


project (OpenFlipper)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6.0" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "4.9" OR CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL "4.9")
		message(WARNING "Your version of GCC contains an optimizer bug. Please verify that you do not use -O3!")
		string(REPLACE "-O3" "-O2" CMAKE_CXX_FLAGS_RELEASE_NEW "${CMAKE_CXX_FLAGS_RELEASE}")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE_NEW}" CACHE STRING "" FORCE)
	endif()
endif()

# add our macro directory to cmake search path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/OpenFlipper/cmake)

# Include some of our OpenFlipper cmake macros
include(OpenFlipper_common)

# search for more macro directories in Package-* folders
file (
  GLOB _more_macro_directories
       "${CMAKE_SOURCE_DIR}/Package-*/cmake"
)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${_more_macro_directories})

# ========================================================================
# QT Setup 
# ========================================================================

# Qt setup
include(ACGQt)

acg_qt5 ()

if (NOT QT5_FOUND)
  message(FATAL_ERROR "Could not found any QT Version. Please specify QT5_INSTALL_PATH (path to bin and include dir) to build with QT5")
endif()

set (OF_MAX_QT_VERSION "5.6.0")

if (${Qt5Core_VERSION_STRING} STRGREATER ${OF_MAX_QT_VERSION})
  message(WARNING "Detected QT Version higher than ${OF_MAX_QT_VERSION}!\n" "OpenFlipper has not been tested with this QT Version.")
endif()

# ========================================================================
# ========================================================================

# include our cmake files
include (ACGCommon)
include (MSVCMacros)

acg_get_version (OpenFlipper)

# ========================================================================
# Branding 
# ========================================================================

# search all branding dirs in the packages and in toplevel
file (
  GLOB _local_branding_in
  RELATIVE "${CMAKE_SOURCE_DIR}"
           "${CMAKE_SOURCE_DIR}/branding"
           "${CMAKE_SOURCE_DIR}/Package-*/branding"
)

# Get number of found branding dirs
list(LENGTH _local_branding_in branding_dir_count)

# Check if we found at least one branding directory
if (  ${branding_dir_count} GREATER 0 )

  list(GET _local_branding_in 0 BRANDING_DIRECTORY_TMP)
  set( BRANDING_DIRECTORY "${CMAKE_SOURCE_DIR}/${BRANDING_DIRECTORY_TMP}" CACHE PATH "Path to the branding files")

  if ( ${branding_dir_count} GREATER 1 )
    message(WARNING "Found multiple branding dirs in : ${_local_branding_in}")
    message(WARNING "Using only first branding directory: ${BRANDING_DIRECTORY}")    
  endif()
  
elseif()
  set( BRANDING_DIRECTORY "branding" CACHE PATH "Path to the branding files")

endif()

if ( EXISTS ${BRANDING_DIRECTORY}/branding.cmake )
	include(${BRANDING_DIRECTORY}/branding.cmake)
endif()

if ( EXISTS ${BRANDING_DIRECTORY}/branding.h )
  set( BRANDING_HEADER true CACHE String "Indicate there is a branding.h inside the branding folder that should be included.")
endif()


#Initialize with defaults, if not already set via branding cmake file
set( OPENFLIPPER_PRODUCT_STRING "OpenFlipper" CACHE String "Product string used e.g. to determine the config file path. Should only contain alphanumeric characters (no spaces)." )
set( OPENFLIPPER_PRODUCT_NAME "OpenFlipper" CACHE String "Product name used in all visible places (e.g. about box)." )

if (BRANDING_HEADER)
add_definitions(-DBRANDING_HEADER)
endif ()

add_definitions(-DPRODUCT_STRING=${OPENFLIPPER_PRODUCT_STRING})
add_definitions(-DPRODUCT_NAME=${OPENFLIPPER_PRODUCT_NAME})

# ========================================================================
# ========================================================================

include (package)
include (ACGOutput)

acg_openmp ()

# check for OpenGL, GLEW and GLUT as our required dependencies
find_package (OpenGL)
if (NOT OPENGL_FOUND)
  message (FATAL_ERROR "OpengGL not found!")
endif ()

find_package (GLEW)
if (NOT GLEW_FOUND)
  message (FATAL_ERROR "GLEW not found!")
endif ()

find_package (GLUT)
if (NOT GLUT_FOUND)
  message (FATAL_ERROR "GLUT not found!")
endif ()

# using the internal openmesh delivered by the svn repo (default) or use some external installation
set (USE_INTERNAL_OPENMESH ON CACHE BOOL "Use and build internal version of OpenMesh found in libs_required")

# enable/disable openmesh warnings
set(NO_DECREMENT_DEPRECATED_WARNINGS OFF CACHE BOOL "Disables all deprecated warnings warning about decrement operations on normal circulators.")
if(NO_DECREMENT_DEPRECATED_WARNINGS)
  add_definitions( -DNO_DECREMENT_DEPRECATED_WARNINGS )
endif()

find_package(ACG)

# OpenMesh is a dependent project (in libs_required) so no finder is needed.
if (NOT USE_INTERNAL_OPENMESH)
  find_package (OpenMesh REQUIRED)
endif()

find_package (OpenVolumeMesh)
if (NOT OPENVOLUMEMESH_FOUND)
    message (WARNING "OpenVolumeMesh not found, proceeding without it!")
endif ()


# ========================================================================
# Configure the executables path for tests
# ========================================================================
if (APPLE)
  SET( OPENFLIPPER_EXECUTABLE "${CMAKE_BINARY_DIR}/Build/OpenFlipper.app/Contents/MacOS/${OPENFLIPPER_PRODUCT_STRING}" )
elseif( WIN32 )
  SET( OPENFLIPPER_EXECUTABLE "${CMAKE_BINARY_DIR}/Build/${OPENFLIPPER_PRODUCT_STRING}.exe" )
else()
  SET( OPENFLIPPER_EXECUTABLE "${CMAKE_BINARY_DIR}/Build/bin/${OPENFLIPPER_PRODUCT_STRING}" )
endif()
SET(OPENFLIPPER_TEST_FILES    ${CMAKE_BINARY_DIR}/tests/globalTestFiles )
SET(OPENFLIPPER_TEST_RESULTS  ${CMAKE_BINARY_DIR}/tests/testResults )
SET(OPENFLIPPER_TEST_BINARIES ${CMAKE_BINARY_DIR}/tests/testBinaries )

enable_testing()

# ========================================================================
# Build all additional libraries
# ========================================================================

add_subdirectory (OpenFlipper/libs_required/OpenMesh)
add_subdirectory (OpenFlipper/libs_required/ACG)

# search all libs in the libs directory
# in Packages, types, ...
# (required and optional libraries )
file (
  GLOB _local_libs_in
  RELATIVE "${CMAKE_SOURCE_DIR}"
           "${CMAKE_SOURCE_DIR}/libs_required/*/CMakeLists.txt"
           "${CMAKE_SOURCE_DIR}/libs/*/CMakeLists.txt"
           "${CMAKE_SOURCE_DIR}/Type-*/libs/*/CMakeLists.txt"
           "${CMAKE_SOURCE_DIR}/Package-*/libs/*/CMakeLists.txt"
)

set( REMAINING_LOCAL_LIBS "" )

# Special Comiso case:
# The library has to be configured first
foreach (_localLib ${_local_libs_in})
  if ( "${_localLib}" MATCHES .*CoMISo.* )
    get_filename_component (_local_lib_dir ${_localLib} PATH)
    if ( NOT DEFINED LIB_COMISO_BUILD )
      set( LIB_COMISO_BUILD true CACHE BOOL "Enable or disable build of COMISO Library" )
    endif()

    if ( LIB_COMISO_BUILD )
      add_subdirectory (${CMAKE_SOURCE_DIR}/${_local_lib_dir})
    endif()
  else()
    if ( NOT "${_localLib}" MATCHES OpenMesh OR USE_INTERNAL_OPENMESH)
      LIST(APPEND REMAINING_LOCAL_LIBS ${_localLib})
    endif()
  endif()
endforeach ()


# add all found libs to the build
foreach (_localLib ${REMAINING_LOCAL_LIBS})

  get_filename_component (_local_lib_dir ${_localLib} PATH)

  # If the library is in the required folder, we have to build it anyway, so
  # detect it here
  SET(REQUIRED FALSE)
  if ( _localLib MATCHES ".*libs_required.*" )
    SET(REQUIRED TRUE )
  endif()

  # Extract upper case library dir name for the human readable entries in the cmake config
  string(TOUPPER ${_local_lib_dir} _local_lib_dir_upper)
  string(REGEX REPLACE "LIBS[/\\]" "" _local_lib_dir_upper ${_local_lib_dir_upper} )

  # Only generate setting if the lib is not required (otherwise, we will always build it)
  if ( NOT DEFINED LIB_${_local_lib_dir_upper}_BUILD AND NOT REQUIRED)
      set( LIB_${_local_lib_dir_upper}_BUILD true CACHE BOOL "Enable or disable build of ${_local_lib_dir_upper} Library" )
  endif()

  # Build if this library is required or if it is enabled
  if ( LIB_${_local_lib_dir_upper}_BUILD OR REQUIRED)
    add_subdirectory (${CMAKE_SOURCE_DIR}/${_local_lib_dir})
  endif()
endforeach ()

# Group the external libraries in a folder libs_required
GROUP_PROJECT(OpenMeshTools libs_required)
GROUP_PROJECT(OpenMeshCore libs_required)
GROUP_PROJECT(ACG libs_required)
GROUP_PROJECT(OpenVolumeMesh libs_required)

if( DOXYGEN_FOUND )
GROUP_PROJECT(doc-install Documentation)
endif()

# ========================================================================
# Core Library Components (Except the Core application)
# ========================================================================
add_subdirectory (OpenFlipper/PluginLib)

# ========================================================================
# Build DataTypes
# ========================================================================

# Search for all available data types
file (
  GLOB _data_types
  RELATIVE "${CMAKE_SOURCE_DIR}"
           "${CMAKE_SOURCE_DIR}/Type-*/ObjectTypes/*"
           "${CMAKE_SOURCE_DIR}/Package-*/Type-*/ObjectTypes/*"
)

# add all found libs to the build
foreach (DATATYPE_DIR ${_data_types})
  if (EXISTS ${CMAKE_SOURCE_DIR}/${DATATYPE_DIR}/CMakeLists.txt)
    add_subdirectory(${DATATYPE_DIR})

    # Extract Name of type from directory
    string(REGEX MATCH "[^/\\]*$" TYPENAME ${DATATYPE_DIR})
    string(TOUPPER ${TYPENAME} TYPENAME )
    list(APPEND TYPE_LIST ${TYPENAME})

    set(${TYPENAME}_INCLUDE ${CMAKE_SOURCE_DIR}/${DATATYPE_DIR}/../../ )
 endif()
endforeach()

# info header for Available data types
acg_color_message ("\n${_escape}[40;37m************************************************************${_escape}[0m")
acg_color_message ("${_escape}[40;37m* Available OpenFlipper Types:                             *${_escape}[0m")
acg_color_message ("${_escape}[40;37m************************************************************${_escape}[0m")
acg_color_message (" ")

foreach (DATATYPE ${TYPE_LIST})
  acg_color_message ("${DATATYPE}")
endforeach()

acg_color_message (" ")

# ========================================================================
# Tests
# ========================================================================
add_subdirectory (OpenFlipper/tests)

# ========================================================================
# Documentation generators
# ========================================================================

# Create dirs to make doxygen happy
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_DATADIR}/Doc )
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_DATADIR}/Help )

# Add the documentation targets
add_subdirectory (OpenFlipper/Documentation/DeveloperTarget)
add_subdirectory (OpenFlipper/Documentation/DeveloperQtHelpTarget)
add_subdirectory (OpenFlipper/Documentation/UserTarget)
add_subdirectory (OpenFlipper/Documentation/UserQtHelpTarget)

# ========================================================================
# PolyLine extension special case
# ========================================================================

find_package (MeshIntegration)

if (MESH_INTEGRATION_FOUND)
  add_definitions( -DEXTENDED_POLY_LINE )
endif()

# ========================================================================
# License Manager
# ========================================================================
if ( NOT DEFINED OPENFLIPPER_ENABLE_LICENSE_MANAGER )
  set( OPENFLIPPER_ENABLE_LICENSE_MANAGER true CACHE BOOL "Enable or disable license management in all plugins" )
endif()

if ( NOT DEFINED OPENFLIPPER_ENABLE_LICENSE_KEY_GENERATOR )
  set( OPENFLIPPER_ENABLE_LICENSE_KEY_GENERATOR true CACHE BOOL "Enable or disable building the license key generators in all plugins" )
endif()

# ========================================================================
# Apple Mac Store Flags
# ========================================================================
if ( APPLE AND NOT DEFINED OPENFLIPPER_BUILD_APP_STORE_COMPLIANT )
  set( OPENFLIPPER_BUILD_APP_STORE_COMPLIANT true CACHE BOOL "Enable app store compliant build (No internal update, different paths,...)" )
endif()

# ========================================================================
# Plugins
# ========================================================================

# start with an empty plugin list
acg_set (OPENFLIPPER_PLUGINS "")
acg_set (OPENFLIPPER_STATIC_PLUGINS "")
acg_set (OPENFLIPPER_STATIC_PLUGIN_FILES "")
acg_set (OPENFLIPPER_STATIC_PLUGIN_NAMES "")

foreach (package ${PREFOUND_PACKAGES})
find_package(${package})
endforeach ()

SET (LOADED_PACKAGES ${PREFOUND_PACKAGES})
LIST (APPEND LOADED_PACKAGES "Doxygen")
# add plugins
of_add_plugins ()
list (REMOVE_DUPLICATES LOADED_PACKAGES)
set (PREFOUND_PACKAGES "${LOADED_PACKAGES}" CACHE INTERNAL "List of packages that will get found at global scope so they don't get found over and over inside the individual packages.")

# ========================================================================
# Build core as we now know all dependencies
# ========================================================================

add_subdirectory (OpenFlipper/CoreApp)


# ========================================================================
# Bundle generation
# ========================================================================

if (WIN32 )

  # Plugins collected a list of required libdirs for getting
  # their DLLS on windows:
  get_property( WINDOWS_COPY_LIBDIRS GLOBAL PROPERTY WINDOWS_LIBRARY_DIR_LIST )

  if ( WINDOWS_COPY_LIBDIRS )
    list(REMOVE_DUPLICATES WINDOWS_COPY_LIBDIRS)
  endif()

  
  # workaround for VS2018 where the OpenMP vcomp90.dll seems to be missing from the path
  if ( CMAKE_GENERATOR MATCHES "^Visual Studio 9.*" )
    INCLUDE(InstallRequiredSystemLibraries)
    set ( OMP_PATH "${MSVC90_REDIST_DIR}/${CMAKE_MSVC_ARCH}/Microsoft.VC90.OPENMP/;${MSVC90_REDIST_DIR}/Debug_NonRedist/${CMAKE_MSVC_ARCH}/Microsoft.VC90.DebugOpenMP/" )
  endif()

  # prepare bundle generation cmake file and add a build target for it
  configure_file ("${CMAKE_SOURCE_DIR}/OpenFlipper/cmake/fixbundle.cmake.win.in"
     "${CMAKE_BINARY_DIR}/fixbundle.win.cmake" @ONLY IMMEDIATE)

  add_custom_target (fixbundle ALL
    COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/fixbundle.win.cmake" )

  # let bundle generation depend on all targets
  add_dependencies (fixbundle ${OPENFLIPPER_PRODUCT_STRING} OpenFlipperPluginLib ${OPENFLIPPER_PLUGINS})

endif(WIN32)

# ========================================================================
# Custom command to create a bundle and a package on mac
# ========================================================================

if ( APPLE )

   # Configuration option about fixbundle
   if ( APPLE AND NOT DEFINED OPENFLIPPER_CALL_FIX_BUNDLE )
     set( OPENFLIPPER_CALL_FIX_BUNDLE false CACHE BOOL "After building OpenFlipper, a bundle creation step is executed (which copies all dependencies into the bundle) to create a valid MacOs Bundle. This step is only necessary if creating a package." )
   endif()

  # prepare bundle generation cmake file and add a build target for it
  configure_file ("${CMAKE_SOURCE_DIR}/OpenFlipper/cmake/fixbundle.cmake.in"
     "${CMAKE_BINARY_DIR}/fixbundle.cmake" @ONLY IMMEDIATE)

  # Fixbundle is only necessary, when we wan't a shippable package.
  # Otherwise the original rpaths are used
  if ( OPENFLIPPER_CALL_FIX_BUNDLE )
    # Call fixbundle every time
    add_custom_target (fixbundle ALL
      COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/fixbundle.cmake"
    )
  else()
    # Call fixbundle only as a dependency
    add_custom_target (fixbundle
      COMMAND ${CMAKE_COMMAND} -P "${CMAKE_BINARY_DIR}/fixbundle.cmake"
    )
  endif()

  # let bundle generation depend on all targets
  add_dependencies (fixbundle ${OPENFLIPPER_PRODUCT_STRING} OpenFlipperPluginLib ${OPENFLIPPER_PLUGINS})

  # install bundle
  install (DIRECTORY ${CMAKE_BINARY_DIR}/Build/OpenFlipper.app DESTINATION .
           USE_SOURCE_PERMISSIONS)

  # Extra target for building an OpenFlipper package on mac
  # Call fixbundle first
  # afterwards we call make package as an external command, as we can't
  # add a dependency on build in targets of cake
  add_custom_target (OpenFlipper_package
    DEPENDS fixbundle
    COMMAND make package
  )

endif()

# ========================================================================
# Output log information
# ========================================================================

# display results
acg_print_configure_header (OPENFLIPPER "OpenFlipper")
of_print_plugin_stats ()
acg_print_configure_footer ()
