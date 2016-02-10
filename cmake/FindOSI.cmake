# - Try to find OSI
# Once done this will define
#  OSI_FOUND - System has OSI
#  OSI_INCLUDE_DIRS - The OSI include directories
#  OSI_LIBRARIES - The libraries needed to use OSI

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


if (NOT OSI_FOUND )

find_path(OSI_INCLUDE_DIR 
          NAMES OsiConfig.h
		  PATH_SUFFIXES "Osi"
          PATHS "$ENV{OSI_DIR}/include/coin"
                "$ENV{CBC_DIR}/include/coin"
                 "/usr/include/coin"
                 "C:\\libs\\osi\\include"
                 "C:\\libs\\cbc\\include"
                 "${VS_SEARCH_PATH}CBC-2.9.7/Osi/include"
                 "${VS_SEARCH_PATH}CBC-2.9.4/Osi/include"
              )

find_library( OSI_LIBRARY_DEBUG 
              NAMES Osid libOsid
              PATHS "$ENV{OSI_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\OSI\\lib"
                    "C:\\libs\\cbc\\lib"
                    "${VS_SEARCH_PATH}CBC-2.9.7/lib/${VS_SUBDIR}Debug"
                    "${VS_SEARCH_PATH}CBC-2.9.4/Osi/lib"
              )
              
find_library( OSI_LIBRARY_RELEASE 
              NAMES Osi libOsi
              PATHS "$ENV{OSI_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\OSI\\lib"
                    "C:\\libs\\cbc\\lib"
                    "${VS_SEARCH_PATH}CBC-2.9.7/lib/${VS_SUBDIR}Release"
                    "${VS_SEARCH_PATH}CBC-2.9.4/Osi/lib"
              )              

find_library( OSI_CBC_LIBRARY_DEBUG 
              NAMES OsiCbcd libOsiCbcd
              PATHS "$ENV{OSI_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\OSI\\lib"
                    "C:\\libs\\cbc\\lib"
                    "${VS_SEARCH_PATH}CBC-2.9.7/lib/${VS_SUBDIR}Debug"
                    "${VS_SEARCH_PATH}CBC-2.9.4/Osi/lib"
              )
              
find_library( OSI_CBC_LIBRARY_RELEASE
              NAMES OsiCbc libOsiCbc
              PATHS "$ENV{OSI_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\OSI\\lib"
                    "C:\\libs\\cbc\\lib"
                    "${VS_SEARCH_PATH}CBC-2.9.7/lib/${VS_SUBDIR}Release"
                    "${VS_SEARCH_PATH}CBC-2.9.4/Osi/lib"
              )              

find_library( OSI_CLP_LIBRARY_DEBUG 
              NAMES OsiClpd libOsiClpd
              PATHS "$ENV{OSI_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\OSI\\lib"
                    "C:\\libs\\cbc\\lib"
                    "${VS_SEARCH_PATH}CBC-2.9.7/lib/${VS_SUBDIR}Debug"
                    "${VS_SEARCH_PATH}CBC-2.9.4/Osi/lib"
              )
              
find_library( OSI_CLP_LIBRARY_RELEASE
              NAMES OsiClp libOsiClp
              PATHS "$ENV{OSI_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\OSI\\lib"
                    "C:\\libs\\cbc\\lib"
                    "${VS_SEARCH_PATH}CBC-2.9.7/lib/${VS_SUBDIR}Release"
                    "${VS_SEARCH_PATH}CBC-2.9.4/Osi/lib"
              )              

              
     
     
     
include(SelectLibraryConfigurations)
select_library_configurations( OSI )           
select_library_configurations( OSI_CBC )           
select_library_configurations( OSI_CLP )           
              

              
set(OSI_INCLUDE_DIRS "${OSI_INCLUDE_DIR}" )
set(OSI_LIBRARIES "${OSI_LIBRARIES};${OSI_CBC_LIBRARIES};${OSI_CLP_LIBRARIES}" )


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set OSI_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(OSI  DEFAULT_MSG
                                  OSI_LIBRARY OSI_CBC_LIBRARY OSI_CLP_LIBRARY OSI_INCLUDE_DIR)

mark_as_advanced(OSI_INCLUDE_DIR OSI_LIBRARY OSI_CBC_LIBRARY OSI_CLP_LIBRARY OSI_LIBRARIES)

endif(NOT OSI_FOUND)
