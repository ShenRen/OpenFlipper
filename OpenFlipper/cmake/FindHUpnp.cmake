# - Try to findHUPNP
# Once done this will define
#
# HUPNP_FOUND           - system has HUPNP
# HUPNP_INCLUDE_DIR - theHUPNP include directory
# HUPNP_LIBRARY         - Link these to use HUPNP
# HUPNP_LIBRARY_DIR  - Library DIR of HUPNP
#

IF (HUPNP_INCLUDE_DIR)
  # Already in cache, be silent
  SET(HUPNP_FIND_QUIETLY TRUE)
ENDIF (HUPNP_INCLUDE_DIR)

FIND_PATH(HUPNP_INCLUDE_DIR
    NAMES "HUpnpCore/HControlPoint"
    PATHS "/usr/include"
          "C:\\libs\\HUpnp\\include")

IF (HUPNP_INCLUDE_DIR )

    FIND_LIBRARY(HUPNP_BASE_LIBRARY 
                  NAMES "HUpnp"
                  PATHS "/usr/lib"
                        "C:\\libs\\HUpnp\\lib"
    )


    FIND_LIBRARY(HUPNP_SOAP_LIBRARY
                  NAMES "QtSolutions_SOAP-2.7"
                  PATHS "/usr/lib"
                        "C:\\libs\\HUpnp\\lib"
    )

    IF(NOT HUPNP_SOAP_LIBRARY)
        #We dont have the actual library file, we try looking for the version shared object library.
        #Use hack mentioned in : http://www.cmake.org/pipermail/cmake/2011-March/043369.html
        #Temporarily add suffix of the library file version
        LIST(APPEND CMAKE_FIND_LIBRARY_SUFFIXES ".so.1.0.0")

        FIND_LIBRARY(HUPNP_SOAP_LIBRARY
            NAMES "QtSolutions_SOAP-2.7"
            PATHS "/usr/lib"
                  "C:\\libs\\HUpnp\\lib"
        )

        #Remove suffix after search is completed
        LIST(REMOVE_ITEM CMAKE_FIND_LIBRARY_SUFFIXES ".so.1.0.0")

    ENDIF()

    IF ( HUPNP_SOAP_LIBRARY ) 
      SET(HUPNP_LIBRARY ${HUPNP_BASE_LIBRARY} ${HUPNP_SOAP_LIBRARY} )
      SET(HUPNP_FOUND TRUE)
    ELSE ()
      message (warning "HUPNP_SOAP_LIBRARY not found")
      SET(HUPNP_FOUND FALSE)
    ENDIF()

ELSE (HUPNP_INCLUDE_DIR)
    SET(HUPNP_FOUND FALSE )
ENDIF (HUPNP_INCLUDE_DIR )

