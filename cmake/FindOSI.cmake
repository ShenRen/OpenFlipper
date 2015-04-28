# - Try to find OSI
# Once done this will define
#  OSI_FOUND - System has OSI
#  OSI_INCLUDE_DIRS - The OSI include directories
#  OSI_LIBRARIES - The libraries needed to use OSI

if (OSI_INCLUDE_DIR)
  # in cache already
  set(OSI_FOUND TRUE)
  set(OSI_INCLUDE_DIRS "${OSI_INCLUDE_DIR}" )
  set(OSI_LIBRARIES "${OSI_LIBRARY};${OSI_CBC_LIBRARY};${OSI_CLP_LIBRARY}" )
else (OSI_INCLUDE_DIR)

find_path(OSI_INCLUDE_DIR 
          NAMES OsiConfig.h
          PATHS "$ENV{OSI_DIR}/include/coin"
                "$ENV{CBC_DIR}/include/coin"
                 "/usr/include/coin"
                 "C:\\libs\\osi\\include"
                 "C:\\libs\\cbc\\include"
          )

find_library( OSI_LIBRARY 
              NAMES Osi

              PATHS "$ENV{OSI_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\OSI\\lib"
                    "C:\\libs\\cbc\\lib"
              )

find_library( OSI_CBC_LIBRARY 
              NAMES OsiCbc

              PATHS "$ENV{OSI_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\OSI\\lib"
                    "C:\\libs\\cbc\\lib"
              )

find_library( OSI_CLP_LIBRARY 
              NAMES OsiClp

              PATHS "$ENV{OSI_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\OSI\\lib"
                    "C:\\libs\\cbc\\lib"
              )

set(OSI_INCLUDE_DIRS "${OSI_INCLUDE_DIR}" )
set(OSI_LIBRARIES "${OSI_LIBRARY};${OSI_CBC_LIBRARY};${OSI_CLP_LIBRARY}" )


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set OSI_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(OSI  DEFAULT_MSG
                                  OSI_LIBRARY OSI_CBC_LIBRARY OSI_CLP_LIBRARY OSI_INCLUDE_DIR)

mark_as_advanced(OSI_INCLUDE_DIR OSI_LIBRARY OSI_CBC_LIBRARY OSI_CLP_LIBRARY)

endif(OSI_INCLUDE_DIR)
