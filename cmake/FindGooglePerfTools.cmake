# - Try to find Google Perftools
# Once done this will define
#  
# TCMALLOC_LIBRARY   		    - path to libtcmalloc.so
# PROFILER_LIBRARY		    - path to libprofiler.so
# GOOGLEPERFTOOLS_FOUND             - system has UTIL
# GOOGLEPERFTOOLS_INCLUDE_DIR       - the UTIL include directory
# GOOGLEPERFTOOLS_LIBRARY           - Link these to use UTIL
# GOOGLEPERFTOOLS_LIBRARY_DIR       - Library DIR of UTIL

FIND_LIBRARY( TCMALLOC_LIBRARY tcmalloc
	/usr/local/lib
	/usr/lib
	)

FIND_LIBRARY( PROFILER_LIBRARY profiler
	/usr/local/lib
	/usr/lib
	)

FIND_PATH( GOOGLEPERFTOOLS_LIBRARY_DIR libtcmalloc_and_profiler.so
	/usr/local/lib
	/usr/lib/
	)

FIND_PATH( GOOGLEPERFTOOLS_INCLUDE_DIR google/heap-profiler.h
	/usr/local/include
	/usr/include
	)

IF ( TCMALLOC_LIBRARY AND PROFILER_LIBRARY AND GOOGLEPERFTOOLS_INCLUDE_DIR AND GOOGLEPERFTOOLS_LIBRARY_DIR )
	SET( GOOGLEPERFTOOLS_FOUND "YES" )
	SET( GOOGLEPERFTOOLS_LIBRARY ${TCMALLOC_LIBRARY} ${PROFILER_LIBRARY} )
ENDIF ( TCMALLOC_LIBRARY AND PROFILER_LIBRARY AND GOOGLEPERFTOOLS_INCLUDE_DIR AND GOOGLEPERFTOOLS_LIBRARY_DIR )

IF ( GOOGLEPERFTOOLS_FOUND )
ELSE ( GOOGLEPERFTOOLS_FOUND )
	MESSAGE( AUTHOR_WARNING "Could not find g(oogle-)perftools library.
	TCMALLOC_LIBRARY: ${TCMALLOC_LIBRARY},
	PROFILER_LIBRARY: ${PROFILER_LIBRARY},
	GOOGLEPERFTOOLS_LIBRARY_DIR: ${GOOGLEPERFTOOLS_LIBRARY_DIR}
	GOOGLEPERFTOOLS_INCLUDE_DIR: ${GOOGLEPERFTOOLS_INCLUDE_DIR}
	GOOGLEPERFTOOLS_LIBRARY: ${GOOGLEPERFTOOLS_LIBRARY}
	")
ENDIF ( GOOGLEPERFTOOLS_FOUND )

MARK_AS_ADVANCED(
	GOOGLEPERFTOOLS_LIBRARY
	GOOGLEPERFTOOLS_LIBRARY_DIR
	GOOGLEPERFTOOLS_INCLUDE_DIR
	)
