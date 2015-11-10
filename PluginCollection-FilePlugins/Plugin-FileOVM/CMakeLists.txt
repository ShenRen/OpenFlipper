include (plugin)

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/PolyhedralMesh/PolyhedralMesh.hh)
  openflipper_plugin (DEPS OpenVolumeMesh)
else()
  message( WARNING "Plugin FileOVM disabled due to missing PolyHedralMesh ObjectType" )
endif ()

find_package(OpenVolumeMesh)
if(OPENVOLUMEMESH_FOUND)
  if(EXISTS ${OPENVOLUMEMESH_INCLUDE_DIR}/OpenVolumeMesh/Mesh/TetrahedralMesh.hh)
    if(EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh)
      add_definitions(-DENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT)
    endif()
  endif()

endif()
