include (plugin)

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/HexahedralMesh)
  openflipper_plugin (DEPS OpenVolumeMesh
                      INSTALLDATA Icons)
endif()

find_package(OpenVolumeMesh)
if(OPENVOLUMEMESH_FOUND)
  if(EXISTS ${OPENVOLUMEMESH_INCLUDE_DIR}/OpenVolumeMesh/Mesh/TetrahedralMesh.hh)
    add_definitions(-DENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT)
  endif()

endif()
