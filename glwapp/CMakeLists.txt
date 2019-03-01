cmake_minimum_required (VERSION 3.0.2)
set(APP_PROJECT glwapp)
project (${APP_PROJECT})
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_DEBUG_POSTFIX "d"
    CACHE STRING "Name suffix for debug builds")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
# find_package(LibGLW REQUIRED)
find_package(SDL2 REQUIRED)

file(GLOB ${APP_PROJECT}_SRC
    "*.h"
    "*.cpp"
)
# Library
add_executable(${APP_PROJECT} ${${APP_PROJECT}_SRC})
message("LIBGLW Debug: ${LIBGLW_LIBRARY_DEBUG}; Release: ${LIBGLW_LIBRARY_RELEASE}")
target_link_libraries (${APP_PROJECT}
    debug ${LIBGLW_LIBRARY_DEBUG}
    optimized ${LIBGLW_LIBRARY_RELEASE})
target_link_libraries(${APP_PROJECT} ${SDL2_LIBRARY} ${SDL2_SDLMAIN_LIBRARY} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

target_include_directories(${APP_PROJECT} 
    PUBLIC ${LIBGLW_INCLUDE_DIR}
    PUBLIC ${SDL2_INCLUDE_DIR}
    PUBLIC ${GLM_INCLUDE_DIRS}
    PUBLIC ${GLEW_INCLUDE_DIR}
    )
target_compile_definitions(${APP_PROJECT} PRIVATE GLM_ENABLE_EXPERIMENTAL=1)


set_target_properties(${APP_PROJECT} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

