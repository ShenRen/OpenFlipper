include (plugin)

if (WIN32)
  openflipper_plugin (DEPS CGAL Boost GMM LAPACK BLAS)
else ()
  openflipper_plugin (DEPS GMM LAPACK BLAS)
endif ()
