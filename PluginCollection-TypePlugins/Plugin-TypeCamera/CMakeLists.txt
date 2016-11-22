include (plugin)

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/Camera/Camera.hh)
  openflipper_plugin (INSTALLDATA Icons Shaders)
else()
  message( WARNING "Plugin TypeCamera disabled due to missing Camera ObjectType" )
endif ()