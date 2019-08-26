cmake_minimum_required (VERSION 3.0)
set(LIB_SOLUTION glw)
set(LIB_PROJECT libglw)
set(APP_PROJECT glwapp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")

set(LIBGLW_LIBRARY_ONLY OFF CACHE BOOL "Build only libglw without the application")

project(${LIB_SOLUTION})

add_subdirectory(libglw)
if (NOT LIBGLW_LIBRARY_ONLY)
    add_subdirectory(glwapp)
    add_dependencies(glwapp libglw)
endif()