# - Try to find CPLEX
# Once done this will define
#  CPLEX_FOUND - System has Cplex
#  CPLEX_INCLUDE_DIRS - The Cplex include directories
#  CPLEX_LIBRARIES - The libraries needed to use Cplex

if (CPLEX_INCLUDE_DIR)
  # in cache already
  set(CPLEX_FOUND TRUE)
else (CPLEX_INCLUDE_DIR)

if (WIN32)
#TODO #######################################################################

   find_path(CPLEX_INCLUDE_DIR NAMES SRC/gurobi_c++.h
     PATHS
     "C:\\libs\\gurobi45"
     ${GUROBI_INCLUDE_PATH}
   )

   find_library( GUROBI_LIBRARY_RELEASE 
                 SuperLU
                 PATHS "C:\\libs\\gurobi45\\lib" )
   find_library( GUROBI_LIBRARY_DEBUG
                   SuperLUd
                   PATHS "C:\\libs\\gurobi45\\lib" )


   set ( GUROBI_LIBRARY "optimized;${GUROBI_LIBRARY_RELEASE};debug;${GUROBI_LIBRARY_DEBUG}" CACHE  STRING "GUROBI Libraries" )

ELSEIF(APPLE)
#TODO #######################################################################
   find_path(GUROBI_INCLUDE_DIR NAMES gurobi_c++.h
	     PATHS "${CMAKE_SOURCE_DIR}/MacOS/Libs/gurobi40"
	     	   ${GUROBI_INCLUDE_PATH}
            )

   find_library( GUROBI_LIBRARY 
                 SuperLU
                 PATHS "${CMAKE_SOURCE_DIR}/MacOS/Libs/gurobi40")

ELSE( WIN32 )
   find_path(CPLEX_INCLUDE_DIR NAMES ilcplex/cplex.h
     PATHS "$ENV{CPLEX_DIR}/cplex/include"
   )
   find_path(CPLEX_CONCERT_INCLUDE_DIR NAMES ilconcert/ilomodel.h
     PATHS "$ENV{CPLEX_DIR}/concert/include"
   )
   
   
#   MESSAGE(STATUS "CPLEX         include dir: ${CPLEX_INCLUDE_DIR}")
#   MESSAGE(STATUS "CPLEX concert include dir: ${CPLEX_CONCERT_INCLUDE_DIR}")

   find_library( CPLEX_LIBRARY 
                 cplex
                 PATHS "$ENV{CPLEX_DIR}/cplex/lib/x86-64_sles10_4.1/static_pic" )

   find_library( CPLEX_ILO_LIBRARY 
                 ilocplex
                 PATHS "$ENV{CPLEX_DIR}/cplex/lib/x86-64_sles10_4.1/static_pic" )

   find_library( CPLEX_CONCERT_LIBRARY 
                 concert
                 PATHS "$ENV{CPLEX_DIR}/concert/lib/x86-64_sles10_4.1/static_pic" )                 
ENDIF()

set(CPLEX_INCLUDE_DIRS "${CPLEX_INCLUDE_DIR};${CPLEX_CONCERT_INCLUDE_DIR}" )
set(CPLEX_LIBRARIES "${CPLEX_LIBRARY};${CPLEX_ILO_LIBRARY};${CPLEX_CONCERT_LIBRARY}" )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBCPLEX_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(CPLEX  DEFAULT_MSG
                                  CPLEX_LIBRARY CPLEX_INCLUDE_DIR)

mark_as_advanced(CPLEX_INCLUDE_DIR CPLEX_LIBRARY )

endif(CPLEX_INCLUDE_DIR)