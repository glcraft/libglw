cmake_minimum_required (VERSION 3.0)
set(LIB_SOLUTION glw)
set(LIB_PROJECT libglw)
set(APP_PROJECT glwapp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(${LIB_SOLUTION})
add_subdirectory("${PROJECT_SOURCE_DIR}/libglw")
# project(${APP_PROJECT})
add_subdirectory("${PROJECT_SOURCE_DIR}/glwapp")
message("(libglw) libglw include : ${LIBGLW_INCLUDE_DIR}")
add_dependencies(${APP_PROJECT} ${LIB_PROJECT})

export(EXPORT glwTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/glw/glwTargets.cmake"
  NAMESPACE glw::
)

include(CMakePackageConfigHelpers)
configure_package_config_file( 
  "cmake/glwConfig.cmake.in" 
  "glwConfig.cmake"
  INSTALL_DESTINATION cmake
  PATH_VARS
    CMAKE_INSTALL_LIBDIR
  )

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/glwConfigVersion.cmake
  VERSION 1.0.0
  COMPATIBILITY SameMajorVersion
  )

### Install Config and ConfigVersion files
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/glwConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/glwConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/glw"
  )
