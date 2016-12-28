include(plugin)

# use Qwt6 for QT5
find_package(Qwt6)

if (QWT6_FOUND)
  add_definitions(-DWITH_QWT)
  openflipper_plugin( DEPS Qwt6
                      OPTDEPS OpenVolumeMesh
                      TYPES POLYMESH TRIANGLEMESH
                      OPT_TYPES BSPLINESURFACE HEXAHEDRALMESH POLYHEDRALMESH 
                    )
else()
  openflipper_plugin( OPTDEPS OpenVolumeMesh 
                      TYPES POLYMESH TRIANGLEMESH HEXAHEDRALMESH POLYHEDRALMESH)
endif()
