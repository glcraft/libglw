cmake_minimum_required (VERSION 2.6)
project (libglw)

set(CMAKE_DEBUG_POSTFIX "d"
    CACHE STRING "Name suffix for debug builds")
set(LIBGLW_WITH_STD_FILESYSTEM ON
    CACHE STRING "std::filesystem integration (for loading shaders)")

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
    "include/libglw/*.h"
    "src/*.cpp"
)
# Library
add_library(libglw ${libglw_SRC})
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.in.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/libglw/config.h")

target_link_libraries(libglw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
target_include_directories(libglw 
    PUBLIC ${GLM_INCLUDE_DIRS}
    PUBLIC ${GLEW_INCLUDE_DIR}
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set_target_properties(libglw PROPERTIES 
    CXX_STANDARD 11
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
    OUTPUT_NAME "libglw")
if (LIBGLW_WITH_STD_FILESYSTEM)
    set_target_properties(libglw PROPERTIES 
        CXX_STANDARD 17
    )
endif()
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
endif()