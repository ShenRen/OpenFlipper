
if(MSVC_MACROS_INCLUDED)
  return()
endif(MSVC_MACROS_INCLUDED)
set(MSVC_MACROS_INCLUDED TRUE)

set (MSVC_GROUPING ON CACHE BOOL "Group Files by folder structure on MSVC.")

# Enable project folders to group targets in solution folders on MSVC.
if(${MSVC_GROUPING})
set_property(GLOBAL PROPERTY USE_FOLDERS ON) 
endif(${MSVC_GROUPING})

MACRO (RECURSE_GROUPS dirname)
if(${MSVC_GROUPING})
     SET(ALL_FILES)
     FILE(GLOB_RECURSE allfiles "${dirname}/*.*")
     SET(ALL_FILES ${ALL_FILES} ${allfiles})
     STRING(REGEX REPLACE ".*/([^/]*)" "\\1" maindir "${dirname}")

     FOREACH(file ${allfiles})
         STRING(REGEX REPLACE "${dirname}/(.*)/[^/]*" "\\1" loopdirname "${file}")
         STRING(REGEX REPLACE "/" "\\\\" loopdirname "${loopdirname}")

         IF(NOT "${file}" MATCHES "${dirname}/(.*)/[^/]*")
             source_group("${maindir}" FILES  ${file})
         ELSE()
             source_group("${maindir}\\${loopdirname}" FILES ${file})
         ENDIF()
     ENDFOREACH()
endif(${MSVC_GROUPING})
ENDMACRO (RECURSE_GROUPS)

MACRO (GROUP_PROJECT targetname groupname)
if(${MSVC_GROUPING})
set_target_properties(${targetname} 
   PROPERTIES 
   FOLDER "${groupname}")
endif(${MSVC_GROUPING})
ENDMACRO (GROUP_PROJECT)