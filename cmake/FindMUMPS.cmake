# - Try to find MUMPS
# Once done this will define
#  MUMPS_FOUND - System has Mumps
#  MUMPS_INCLUDE_DIRS - The Mumps include directories
#  MUMPS_LIBRARY_DIRS - The library directories needed to use Mumps
#  MUMPS_LIBRARIES    - The libraries needed to use Mumps

if (MUMPS_INCLUDE_DIR)
  # in cache already
  SET(MUMPS_FIND_QUIETLY TRUE)
endif (MUMPS_INCLUDE_DIR)

# I8 Search paths for windows libraries
if ( CMAKE_GENERATOR MATCHES "^Visual Studio 11.*Win64" )
  SET(VS_SEARCH_PATH "c:/libs/vs2012/x64/")
elseif ( CMAKE_GENERATOR MATCHES "^Visual Studio 11.*" )
  SET(VS_SEARCH_PATH "c:/libs/vs2012/x32/")
elseif ( CMAKE_GENERATOR MATCHES "^Visual Studio 12.*Win64" )
  SET(VS_SEARCH_PATH "c:/libs/vs2013/x64/")
  SET(VS_SUBDIR "x64-v120-")
elseif ( CMAKE_GENERATOR MATCHES "^Visual Studio 12.*" )
  SET(VS_SEARCH_PATH "c:/libs/vs2013/x32/")
endif()

find_path(MUMPS_INCLUDE_DIR NAMES dmumps_c.h
     PATHS "$ENV{IPOPT_HOME}/ThirdParty/Mumps/MUMPS/include/"
           "/usr/include/"
           "${VS_SEARCH_PATH}Ipopt-3.12.4/Ipopt/MSVisualStudio/v8-ifort/installed/include/mumps/include/"
   )
   
find_library( MUMPS_LIBRARY_DEBUG
              dmumpsd coinmumpsd coinmumpscd
              PATHS "$ENV{IPOPT_HOME}/lib/"
                    "/usr/lib"
                    "${VS_SEARCH_PATH}Ipopt-3.12.4/Ipopt/MSVisualStudio/v8-ifort/installed/lib"
                    )
                    
find_library( MUMPS_LIBRARY_RELEASE
              dmumps coinmumps coinmumpsc
              PATHS "$ENV{IPOPT_HOME}/lib/"
                    "/usr/lib"
                    "${VS_SEARCH_PATH}Ipopt-3.12.4/Ipopt/MSVisualStudio/v8-ifort/installed/lib"
                    )                    

include(SelectLibraryConfigurations)
select_library_configurations( MUMPS )                 
                    
set(MUMPS_INCLUDE_DIRS "${MUMPS_INCLUDE_DIR}" )
set(MUMPS_LIBRARIES "${MUMPS_LIBRARY}" )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBCPLEX_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(MUMPS  DEFAULT_MSG
                                  MUMPS_LIBRARY MUMPS_INCLUDE_DIR)

mark_as_advanced(MUMPS_INCLUDE_DIR MUMPS_LIBRARY )