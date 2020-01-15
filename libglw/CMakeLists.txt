cmake_minimum_required (VERSION 2.6)
set(LIB_PROJECT libglw)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_DEBUG_POSTFIX "d"
    CACHE STRING "Name suffix for debug builds")

# set(LIBGLW_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/.." PARENT_SCOPE)
# set(LIBGLW_LIBRARY_DEBUG "${PROJECT_BINARY_DIR}/libglwd.lib" PARENT_SCOPE)
# set(LIBGLW_LIBRARY_RELEASE "${PROJECT_BINARY_DIR}/libglw.lib" PARENT_SCOPE)

# mark_as_advanced(LIBGLW_INCLUDE_DIR LIBGLW_LIBRARY_DEBUG LIBGLW_LIBRARY_RELEASE)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

file(GLOB LIBGLW_SRC
    "src/*.cpp"
)
file(GLOB LIBGLW_HEADERS
    "include/libglw/*.h"
    "include/libglw/*.hpp"
)
# Library
add_library(libglw ${LIBGLW_SRC} ${LIBGLW_HEADERS})
add_library(glw::libglw ALIAS libglw)

target_link_libraries(libglw ${OPENGL_LIBRARIES} GLEW::GLEW glm)

target_include_directories(libglw PUBLIC include/)

set_target_properties(libglw PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

include(GNUInstallDirs)
install( 
  TARGETS libglw
  EXPORT glwTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
