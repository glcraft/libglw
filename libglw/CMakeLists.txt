cmake_minimum_required (VERSION 2.6)
set(LIB_PROJECT libglw)
project (${LIB_PROJECT})
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

file(GLOB ${LIB_PROJECT}_SRC
    "*.h"
    "*.cpp"
)
# Library
add_library(${LIB_PROJECT} ${libglw_SRC})

target_link_libraries(${LIB_PROJECT} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
target_include_directories(${LIB_PROJECT} PUBLIC ${GLM_INCLUDE_DIRS})

set_target_properties(${LIB_PROJECT} PROPERTIES 
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    OUTPUT_NAME "glw")