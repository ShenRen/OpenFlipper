# - Try to find COINUTILS
# Once done this will define
#  COINUTILS_FOUND - System has COINUTILS
#  COINUTILS_INCLUDE_DIRS - The COINUTILS include directories
#  COINUTILS_LIBRARIES - The libraries needed to use COINUTILS

if (COINUTILS_INCLUDE_DIR)
  # in cache already
  set(COINUTILS_FOUND TRUE)
  set(COINUTILS_INCLUDE_DIRS "${COINUTILS_INCLUDE_DIR}" )
  set(COINUTILS_LIBRARIES "${COINUTILS_LIBRARY}" )
else (COINUTILS_INCLUDE_DIR)

find_path(COINUTILS_INCLUDE_DIR 
          NAMES CoinUtilsConfig.h
          PATHS "$ENV{COINUTILS_DIR}/include/coin"
                "$ENV{CBC_DIR}/include/coin"
                 "/usr/include/coin"
                 "C:\\libs\\coinutils\\include"
                 "C:\\libs\\cbc\\include"
          )

find_library( COINUTILS_LIBRARY 
              NAMES CoinUtils

              PATHS "$ENV{COINUTILS_DIR}/lib"
                    "$ENV{CBC_DIR}/lib" 
                    "/usr/lib"
                    "/usr/lib/coin"
                    "C:\\libs\\coinutils\\lib"
                    "C:\\libs\\cbc\\lib"
              )

set(COINUTILS_INCLUDE_DIRS "${COINUTILS_INCLUDE_DIR}" )
set(COINUTILS_LIBRARIES "${COINUTILS_LIBRARY}" )


include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set COINUTILS_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(COINUTILS  DEFAULT_MSG
                                  COINUTILS_LIBRARY COINUTILS_INCLUDE_DIR)

mark_as_advanced(COINUTILS_INCLUDE_DIR COINUTILS_LIBRARY)

endif(COINUTILS_INCLUDE_DIR)
