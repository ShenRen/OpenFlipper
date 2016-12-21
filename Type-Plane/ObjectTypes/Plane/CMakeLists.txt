include(ACGCommon)

include_directories (
  ..
  ${ACG_INCLUDE_DIR}
  ${OPENMESH_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
  ${GLUT_INCLUDE_DIR}
)

link_directories (
  ${GLEW_LIBRARY_DIR}
  ${ADDITIONAL_PLUGINLIB_LINK_DIRS}
)

#=======================================================
# General defines
#=======================================================

# generate dllexport macros on windows
if (WIN32)
  add_definitions (
    -DPLUGINLIBDLL
    -DACGDLL
    -DUSEACG
  )
endif ()

#=======================================================
# Cleanup
#=======================================================

# remove template cc files from source file list
acg_drop_templates (sources)



# collect all header,source files
acg_append_files (headers "*.hh" . )
acg_append_files (sources "*.cc" . )

# Match our current directory name
string (REGEX MATCH "[^/\\]*$" TYPENAME ${CMAKE_CURRENT_SOURCE_DIR})
string(TOUPPER ${TYPENAME} TYPENAME )

include_directories (
  ..
  ${ACG_INCLUDE_DIR}
  ${OPENMESH_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../..
  ${CMAKE_CURRENT_BINARY_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
  ${GLUT_INCLUDE_DIR}
)

acg_add_library ( ${TYPENAME} SHARED ${sources} ${headers} )

add_dependencies( ${TYPENAME} OpenMeshCore OpenMeshTools ACG OpenFlipperPluginLib)


target_link_libraries (${TYPENAME}
  ${OPENMESH_LIBRARIES}
  ACG
  ${QT_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARY}
  ${GLUT_LIBRARIES}
  PluginLib
)

