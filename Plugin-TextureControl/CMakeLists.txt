include(plugin)

SET( CURRENT_PLUGIN_DEPS "")

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/PolyhedralMesh)
    add_definitions (-DENABLE_OPENVOLUMEMESH_SUPPORT)
    add_definitions (-DENABLE_OPENVOLUMEMESH_POLYHEDRAL_SUPPORT)

    list( APPEND CURRENT_PLUGIN_DEPS OpenVolumeMesh )
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/HexahedralMesh)
    add_definitions (-DENABLE_OPENVOLUMEMESH_SUPPORT)
    add_definitions (-DENABLE_OPENVOLUMEMESH_HEXAHEDRAL_SUPPORT)    

    list( APPEND CURRENT_PLUGIN_DEPS OpenVolumeMesh )
endif()

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/BSplineSurface)
    add_definitions (-DENABLE_BSPLINESURFACE_SUPPORT)
endif()

list(REMOVE_DUPLICATES CURRENT_PLUGIN_DEPS )

# use Qwt6 for QT5
find_package(Qwt6)

if (QWT6_FOUND)
  add_definitions(-DWITH_QWT)
  openflipper_plugin( DEPS ${CURRENT_PLUGIN_DEPS} Qwt6
                      TYPES POLYMESH TRIANGLEMESH)
else()
  openflipper_plugin( DEPS ${CURRENT_PLUGIN_DEPS}
                      TYPES POLYMESH TRIANGLEMESH)
endif()
