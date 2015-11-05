# Try to find MeshIntegration
# Once done this will define

# MESH_INTEGRATION_FOUND           - system has MeshIntegration
# MESH_INTEGRATION_INCLUDE_DIRS    - the MeshIntegration include directories

IF (MESH_INTEGRATION_INCLUDE_DIRS)
  # Already in cache, be silent
  SET(MESH_INTEGRATION_FIND_QUIETLY TRUE)
ENDIF (MESH_INTEGRATION_INCLUDE_DIRS)

file (
  GLOB _libdirs
           "${CMAKE_SOURCE_DIR}/libs/MeshIntegration"
           "${CMAKE_SOURCE_DIR}/Package-*/libs/MeshIntegration"
)

FIND_PATH( MESH_INTEGRATION_INCLUDE_DIR PolyLineIntegrationT.hh
           PATHS "${_libdirs}"
                 "${CMAKE_SOURCE_DIR}/libs" )

INCLUDE (FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(MeshIntegration DEFAULT_MSG MESH_INTEGRATION_INCLUDE_DIR)

IF (MESHINTEGRATION_FOUND)
  SET(MESH_INTEGRATION_INCLUDE_DIRS "${MESH_INTEGRATION_INCLUDE_DIR}")
  SET(MESH_INTEGRATION_FOUND TRUE)
ENDIF (MESHINTEGRATION_FOUND)
