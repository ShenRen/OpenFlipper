# - Try to find CGL
# Once done this will define
#  CGL_FOUND - System has CGL
#  CGL_INCLUDE_DIRS - The CGL include directories
#  CGL_LIBRARIES - The libraries needed to use CGL

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

if ( NOT CGL_FOUND)

find_path(CGL_INCLUDE_DIR 
          NAMES CglConfig.h
          PATHS "$ENV{CGL_DIR}/include/coin"
                "$ENV{CBC_DIR}/include/coin"
                 "/usr/include/coin"
                 "C:\\libs\\cgl\\include"
                 "C:\\libs\\cbc\\include"
                 "${VS_SEARCH_PATH}CBC-2.9.7/Cgl/include"
                 "${VS_SEARCH_PATH}CBC-2.9.4/Cgl/include"
              )

find_library( CGL_LIBRARY_DEBUG 
              NAMES Cgld libCgld
              PATHS "$ENV{CGL_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\cgl\\lib"
                    "C:\\libs\\cbc\\lib"
                    "${VS_SEARCH_PATH}CBC-2.9.7/lib/${VS_SUBDIR}Debug"
                    "${VS_SEARCH_PATH}CBC-2.9.4/Cgl/lib"
              )
              
find_library( CGL_LIBRARY_RELEASE
              NAMES Cgl libCgl
              PATHS "$ENV{CGL_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\cgl\\lib"
                    "C:\\libs\\cbc\\lib"
                    "${VS_SEARCH_PATH}CBC-2.9.7/lib/${VS_SUBDIR}Release"
                    "${VS_SEARCH_PATH}CBC-2.9.4/Cgl/lib"
              )              

include(SelectLibraryConfigurations)
select_library_configurations( CGL )
  
set(CGL_INCLUDE_DIRS "${CGL_INCLUDE_DIR}" )
set(CGL_LIBRARIES "${CGL_LIBRARY}" )


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CGL_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(CGL  DEFAULT_MSG
                                  CGL_LIBRARY CGL_INCLUDE_DIR)

mark_as_advanced(CGL_INCLUDE_DIR CGL_LIBRARY)

endif(NOT CGL_FOUND)
