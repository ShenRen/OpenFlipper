include (plugin)

if (EXISTS ${CMAKE_SOURCE_DIR}/ObjectTypes/PolyLineCollection/PolyLineCollection.hh)
  openflipper_plugin (INSTALLDATA Icons )
else()
  message( WARNING "Plugin TypePolyLineCollection disabled due to missing ObjectType" )
endif ()

