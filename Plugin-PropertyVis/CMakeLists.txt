include (plugin)

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/BSplineCurve)
  add_definitions (-DENABLE_SKELETON_SUPPORT)
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/PolyhedralMesh)
    add_definitions (-DENABLE_OPENVOLUMEMESH_SUPPORT)
    add_definitions (-DENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT)
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/HexahedralMesh)
    add_definitions (-DENABLE_OPENVOLUMEMESH_SUPPORT)
    add_definitions (-DENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT)
endif()

find_package(OpenVolumeMesh)
if(OPENVOLUMEMESH_FOUND)
  if(EXISTS ${OPENVOLUMEMESH_INCLUDE_DIR}/OpenVolumeMesh/Mesh/TetrahedralMesh.hh)
    if(EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/TetrahedralMesh/TetrahedralMesh.hh)
      add_definitions (-DENABLE_OPENVOLUMEMESH_SUPPORT)
      add_definitions (-DENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT)
    endif()
  endif()
endif()


openflipper_plugin (INSTALLDATA Icons DIRS OpenMesh OpenVolumeMesh Widgets DEPS OpenMesh OpenVolumeMesh)
