include (plugin)

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/HexahedralMesh)
    add_definitions (-DENABLE_OPENVOLUMEMESH_SUPPORT)
    add_definitions (-DENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT)
endif ()

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/TetrahedralMesh)
    add_definitions (-DENABLE_OPENVOLUMEMESH_SUPPORT)
    add_definitions (-DENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT)
endif ()

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/PolyhedralMesh)
    add_definitions (-DENABLE_OPENVOLUMEMESH_SUPPORT)
    add_definitions (-DENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT)
endif ()

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/TetrahedralMesh)
    add_definitions (-DENABLE_OPENVOLUMEMESH_SUPPORT)
    add_definitions (-DENABLE_OPENVOLUMEMESH_TETRAHEDRAL_SUPPORT)
endif ()

openflipper_plugin (DEPS OpenVolumeMesh
                    TYPES POLYMESH TRIANGLEMESH
                    OPT_TYPES BSPLINECURVE BSPLINESURFACE)
