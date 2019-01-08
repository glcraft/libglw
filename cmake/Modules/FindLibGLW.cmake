#
# Find GLM
#
# Try to find GLM : OpenGL Mathematics.
# This module defines 
# - GLM_INCLUDE_DIRS
# - GLM_FOUND
#
# The following variables can be set as arguments for the module.
# - GLM_ROOT_DIR : Root library directory of GLM 
#
# References:
# - https://github.com/Groovounet/glm/blob/master/util/FindGLM.cmake
# - https://bitbucket.org/alfonse/gltut/src/28636298c1c0/glm-0.9.0.7/FindGLM.cmake
#

# Additional modules
include(FindPackageHandleStandardArgs)

# Invoke pkgconfig for hints
find_package(PkgConfig QUIET)
set(LIBGLW_INCLUDE_HINTS)
set(LIBGLW_LIB_HINTS)
if(PKG_CONFIG_FOUND)
	pkg_search_module(LIBGLWPC QUIET LIBGLW)
	if(LIBGLWPC_INCLUDE_DIRS)
		set(LIBGLW_INCLUDE_HINTS ${LIBGLWPC_INCLUDE_DIRS})
	endif()
	if(LIBGLWPC_LIBRARY_DIRS)
		set(LIBGLW_LIB_HINTS ${LIBGLWPC_LIBRARY_DIRS})
	endif()
endif()

find_library(LIBGLW_LIBRARY_DEBUG
	NAMES
    libglwd
    HINTS
    ${LIBGLW_LIB_HINTS}
	PATHS
	${LIBGLW_ROOT_DIR}
	ENV LIBGLWDIR
    PATH_SUFFIXES lib LIBGLW ${LIBGLW_LIB_PATH_SUFFIX})
find_library(LIBGLW_LIBRARY_RELEASE
	NAMES libglw
    HINTS ${LIBGLW_LIB_HINTS}
	PATHS
	${LIBGLW_ROOT_DIR}
	ENV LIBGLWDIR
    PATH_SUFFIXES lib LIBGLW ${LIBGLW_LIB_PATH_SUFFIX})
    
if (WIN32)
	# Find include files
	find_path(
		LIBGLW_INCLUDE_DIR
		NAMES libglw/GLWrapper.h
		PATHS
		$ENV{PROGRAMFILES}/include
		${GLM_ROOT_DIR}/include
		DOC "The directory where libglw/GLWrapper.h resides")
else()
	# Find include files
	find_path(
		LIBGLW_INCLUDE_DIR
		NAMES libglw/GLWrapper.h
		PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		${GLM_ROOT_DIR}/include
		DOC "The directory where libglw/GLWrapper.h resides")
endif()

# Handle REQUIRD argument, define *_FOUND variable
find_package_handle_standard_args(LIBGLW DEFAULT_MSG LIBGLW_INCLUDE_DIR)

# Hide some variables
mark_as_advanced(LIBGLW_INCLUDE_DIR)