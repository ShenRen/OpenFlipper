include (plugin)

if (WIN32)
  openflipper_plugin (TYPES TRIANGLEMESH POLYMESH DEPS CGAL Boost GMM LAPACK BLAS)
else ()
  openflipper_plugin (TYPES TRIANGLEMESH POLYMESH DEPS GMM LAPACK BLAS)
endif ()
