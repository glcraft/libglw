cmake_minimum_required (VERSION 3.0)
set(LIB_SOLUTION glw)
set(LIB_PROJECT libglw)
set(APP_PROJECT glwapp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(${LIB_SOLUTION})
add_subdirectory("${PROJECT_SOURCE_DIR}/libglw")
# project(${APP_PROJECT})
add_subdirectory("${PROJECT_SOURCE_DIR}/glwapp")
# add_dependencies(${APP_PROJECT} ${LIB_PROJECT})
