# Find glfw3
#
# Finds the libraries and header files for the glfw3 SDK 
#
# This module defines
# GLFW3_FOUND       - glfw3 was found
# GLFW3_INCLUDE_DIR - Directory containing GLFW3 header files
# GLFW3_LIBRARY     - Library name of glfw3 library


SET (current_version 3.2.1)

# Don't be verbose if previously run successfully
IF(GLFW3_INCLUDE_DIR AND GLFW3_LIBRARY)
   SET(GLFW3_FIND_QUIETLY TRUE)
ENDIF(GLFW3_INCLUDE_DIR AND GLFW3_LIBRARY)

# Set locations to search
IF(UNIX)
   SET(GLFW3_INCLUDE_SEARCH_DIRS
      /usr/include
      /usr/local/include
	  /usr/local/Cellar/
      /opt/include
      ~/usr/include
      ~/.local/include INTERNAL)
   SET(GLFW3_INC_DIR_SUFFIXES PATH_SUFFIXES glfw3 "glfw3/${current_version}" "glfw3/${current_version}")
   SET(GLFW3_LIBRARY_SEARCH_DIRS
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
	  /usr/local/Cellar
      ~/usr/lib
      ~/usr/lib64
      ~/.local/lib INTERNAL)
ELSE(UNIX)
   # WIN32
   SET(GLFW3_INC_DIR_SUFFIXES PATH_SUFFIXES include)
   if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
       set(_LIB_DIR "x64")
   else()
       set(_LIB_DIR "x86")
   endif()
   SET(GLFW3_LIB_DIR_SUFFIXES PATH_SUFFIXES glfw3/lib/${_LIB_DIR} "glfw3/${current_version}/lib/${_LIB_DIR}")
ENDIF(UNIX)

# Set name of the glfw3 library to use
IF(UNIX)
   IF(APPLE)
      SET(GLFW3_LIBRARY_NAME libglfw.3.dylib)
   ELSE(APPLE)
      SET(GLFW3_LIBRARY_NAME libglfw.3.so)
   ENDIF(APPLE)
ELSE(UNIX)
   # TODO Different libraries are provided for compile and runtime
   SET(GLFW3_LIBRARY_NAME glfw3.lib)
ENDIF(UNIX)

# Search for header files
FIND_PATH(GLFW3_INCLUDE_DIR GLFW/glfw3.h
    PATHS ${GLFW3_INCLUDE_SEARCH_DIRS} ${GLFW3_INCLUDE_SEARCH_PATHS} ${GLFW3_ROOT}
    PATH_SUFFIXES ${GLFW3_INC_DIR_SUFFIXES})

# Search for library
FIND_LIBRARY(GLFW3_LIBRARY ${GLFW3_LIBRARY_NAME}
    PATHS ${GLFW3_LIBRARY_SEARCH_DIRS} ${GLFW3_INCLUDE_SEARCH_PATHS} ${GLFW3_ROOT}
    PATH_SUFFIXES ${GLFW3_LIB_DIR_SUFFIXES})

SET(GLFW3_INCLUDE_DIR ${GLFW3_INCLUDE_DIR} CACHE STRING
    "Directory containing GLFW3 header files")
SET(GLFW3_LIBRARY ${GLFW3_LIBRARY} CACHE STRING "Library name of glfw3 library")

INCLUDE ( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS(glfw3
    DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)
MARK_AS_ADVANCED(GLFW3_INCLUDE_DIR GLFW3_LIBRARY)
