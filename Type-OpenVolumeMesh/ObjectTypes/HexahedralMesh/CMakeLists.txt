find_package(OpenVolumeMesh)

if (OPENVOLUMEMESH_FOUND)
  include_directories(${OPENVOLUMEMESH_INCLUDE_DIR})
  include(type)
  target_link_libraries (${TYPENAME} ${OPENVOLUMEMESH_LIBRARY})
endif()


