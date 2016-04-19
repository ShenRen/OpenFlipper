include (plugin)

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/GCode/GCode.hh)
  openflipper_plugin (INSTALLDATA Icons)
else()
  message( WARNING "Plugin TypeGCode disabled due to missing GCode ObjectType" )
endif ()

