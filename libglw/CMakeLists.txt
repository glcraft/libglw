cmake_minimum_required (VERSION 2.6)
project (libglw)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_DEBUG_POSTFIX "d"
    CACHE STRING "Name suffix for debug builds")

if (WIN32)
    set(LIB_EXT "lib")
else()
    set(LIB_EXT "a")
endif()
set(LIBGLW_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/.." PARENT_SCOPE)
set(LIBGLW_LIBRARY_DEBUG "${PROJECT_BINARY_DIR}/libglwd.${LIB_EXT}" PARENT_SCOPE)
set(LIBGLW_LIBRARY_RELEASE "${PROJECT_BINARY_DIR}/libglw.${LIB_EXT}" PARENT_SCOPE)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)

file(GLOB libglw_SRC
    "*.h"
    "*.cpp"
)
# Library
add_library(libglw ${libglw_SRC})

target_link_libraries(libglw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
target_include_directories(libglw PUBLIC ${GLM_INCLUDE_DIRS})

set_target_properties(libglw PROPERTIES 
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    OUTPUT_NAME "glw")