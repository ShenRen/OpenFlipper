include (plugin)

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/TSplineMesh)
  add_definitions (-DENABLE_TSPLINEMESH_SUPPORT)
endif ()

openflipper_plugin ( TYPES PLANE POLYMESH TRIANGLEMESH
                     OPTDEPS OpenVolumeMesh
                     OPT_TYPES POLYLINE SKELETON HEXAHEDRALMESH POLYHEDRALMESH TETRAHEDRALMESH
		     TRANSLATION_LANGUAGES de_DE
                     INSTALLDATA Icons VsiMetadata )


