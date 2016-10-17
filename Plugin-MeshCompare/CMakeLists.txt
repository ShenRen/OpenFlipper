include(plugin)

find_package(Qwt6)

if (QWT6_FOUND)
  add_definitions(-DWITH_QWT)
  openflipper_plugin(DEPS Qwt6
    INSTALLDATA Icons
  )
else()
  openflipper_plugin(INSTALLDATA Icons)
endif()

