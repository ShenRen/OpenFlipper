include (plugin)

find_package(OpenVolumeMesh)
if(OPENVOLUMEMESH_FOUND)
  if(EXISTS ${OPENVOLUMEMESH_INCLUDE_DIR}/OpenVolumeMesh/Mesh/TetrahedralMesh.hh)
    if(EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh)
      openflipper_plugin (DEPS OpenVolumeMesh)
      add_definitions(-DENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT)
    else()
      message( WARNING "Plugin TypeTetrahedralMesh disabled due to missing TetrahedralMesh ObjectType" )
    endif()
  endif()

endif()
