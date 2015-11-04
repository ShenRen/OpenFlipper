# - Try to find PHYSIM
# Once done this will define
#  
#  PHYSIM_FOUND        - system has PHYSIM
#  PHYSIM_INCLUDE_DIRS - the PHYSIM include directory
#  PHYSIM_LIBRARY_DIR  - where the libraries are
#  PHYSIM_LIBRARY      - Link these to use PHYSIM
#   
IF (PHYSIM_INCLUDE_DIRS)
  # Already in cache, be silent
  SET(PHYSIM_FIND_QUIETLY TRUE)
ENDIF (PHYSIM_INCLUDE_DIRS)

# search all lib directories in packages for OpenFlipper
file (
  GLOB _libdirs
           "${CMAKE_SOURCE_DIR}/libs"
           "${CMAKE_SOURCE_DIR}/Package-*/libs"
)


FIND_PATH( PHYSIM_INCLUDE_DIRS PhySim/Config/PhySimDefines.hh
           PATHS "${_libdirs}" 
                 "${CMAKE_SOURCE_DIR}/libs" )

add_definitions (-DPHYSIMDLL -DUSEPHYSIM )

if (QT5_FOUND)
	SET( PHYSIM_DEPS "Qwt6;GMP" )
else ()
	SET( PHYSIM_DEPS "Qwt5;GMP" )
endif ()

IF (PHYSIM_INCLUDE_DIRS)
  SET(PHYSIM_FOUND TRUE)
  SET( PHYSIM_LIBRARY_DIR "${CMAKE_BINARY_DIR}/Build/${ACG_PROJECT_LIBDIR}" )
  SET( PHYSIM_LIBRARY "PhySim")
  list (APPEND PHYSIM_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/libs/PhySim")
ELSE (PHYSIM_INCLUDE_DIRS)
  SET( PHYSIM_FOUND FALSE )
  SET( PHYSIM_LIBRARY_DIR )
ENDIF (PHYSIM_INCLUDE_DIRS)
