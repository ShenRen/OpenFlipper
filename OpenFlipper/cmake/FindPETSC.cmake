IF (PETSC_INCLUDE_DIRS)
  # Already in cache, be silent
  SET(PETSC_FIND_QUIETLY TRUE)
ENDIF (PETSC_INCLUDE_DIRS)

SET (PETSC_HEADER "petsc.h")
SET (PETSC_LIB_OPT "petsc")
SET (PETSC_LIB_DBG "${PETSC_LIB_OPT}")
IF (WIN32)
    SET (PETSC_LIB_OPT "SuperLU")
    SET (PETSC_LIB_DBG "SuperLUd")
ELSEIF (APPLE)
    SET (PETSC_HEADER "gurobi_c++.h")
    SET (PETSC_LIB_OPT "SuperLU")
    SET (PETSC_LIB_DBG "SuperLU")
ENDIF (WIN32)

FIND_PATH(PETSC_DIR NAMES include/${PETSC_HEADER}
    PATHS
    $ENV{PETSC_DIR}
    /usr/lib/petscdir
    )

FIND_PATH(PETSC_INCLUDE_DIR NAMES ${PETSC_HEADER}
    PATHS
    ${PETSC_DIR}/include
    ${PETSC_INCLUDE_PATH}
    /usr/local/include/petsc
    /usr/local/include
    /usr/include/petsc
    /usr/include
    "C:\\libs\\gurobi45"
    "${CMAKE_SOURCE_DIR}/MacOS/Libs/gurobi40"
)

SET(PETSC_ARCH $ENV{PETSC_ARCH} CACHE PATH "The PETSC architecture.")
FIND_PATH(PETSC_ARCH_INCLUDE_DIR NAMES petscconf.h
    PATHS
    ${PETSC_DIR}/${PETSC_ARCH}/include
)

IF (EXISTS ${PETSC_DIR}/${PETSC_ARCH}/lib)
    SET(PETSC_LIBRARY_DIR ${PETSC_DIR}/${PETSC_ARCH}/lib
        CACHE PATH "The directory where the petsc library resides.")
ELSE()
    SET(PETSC_LIBRARY_DIR NOTFOUND
        CACHE PATH "The directory where the petsc library resides.")
ENDIF()
FIND_LIBRARY( PETSC_LIBRARY_DEBUG
    NAMES ${PETSC_LIB_DBG}
    HINTS
    ${PETSC_LIBRARY_DIR}
    "${CMAKE_SOURCE_DIR}/MacOS/Libs/gurobi40"
    )
FIND_LIBRARY( PETSC_LIBRARY_RELEASE
    NAMES ${PETSC_LIB_OPT}
    HINTS
    ${PETSC_LIBRARY_DIR}
    "${CMAKE_SOURCE_DIR}/MacOS/Libs/gurobi40"
    )
INCLUDE (FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(Petsc DEFAULT_MSG PETSC_LIBRARY_DEBUG PETSC_LIBRARY_RELEASE PETSC_INCLUDE_DIR)

IF (PETSC_FOUND)
    SET(PETSC_LIBRARIES "${PETSC_LIBRARY}")
    SET(PETSC_INCLUDE_DIRS "${PETSC_INCLUDE_DIR}")
    IF (EXISTS ${PETSC_ARCH_INCLUDE_DIR})
        LIST(APPEND PETSC_INCLUDE_DIRS "${PETSC_ARCH_INCLUDE_DIR}")
    ENDIF()
    SET( PETSC_LIBRARY
        debug ${PETSC_LIBRARY_DEBUG}
        optimized ${PETSC_LIBRARY_RELEASE} )
ENDIF (PETSC_FOUND)