# - Try to find CLP
# Once done this will define
#  CLP_FOUND - System has CLP
#  CLP_INCLUDE_DIRS - The CLP include directories
#  CLP_LIBRARIES - The libraries needed to use CLP

if (CLP_INCLUDE_DIR)
  # in cache already
  set(CLP_FOUND TRUE)
  set(CLP_INCLUDE_DIRS "${CLP_INCLUDE_DIR}" )
  set(CLP_LIBRARIES "${CLP_LIBRARY}" )
else (CLP_INCLUDE_DIR)

find_path(CLP_INCLUDE_DIR 
          NAMES ClpConfig.h
          PATHS "$ENV{CLP_DIR}/include/coin"
                "$ENV{CBC_DIR}/include/coin"
                 "/usr/include/coin"
                 "C:\\libs\\clp\\include"
                 "C:\\libs\\cbc\\include"
          )

find_library( CLP_LIBRARY 
              NAMES Clp

              PATHS "$ENV{CLP_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\clp\\lib"
                    "C:\\libs\\cbc\\lib"
              )

set(CLP_INCLUDE_DIRS "${CLP_INCLUDE_DIR}" )
set(CLP_LIBRARIES "${CLP_LIBRARY}" )


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CLP_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(CLP  DEFAULT_MSG
                                  CLP_LIBRARY CLP_INCLUDE_DIR)

mark_as_advanced(CLP_INCLUDE_DIR CLP_LIBRARY)

endif(CLP_INCLUDE_DIR)
