# Find Assimp
#
# Finds the libraries and header files for the Assimp SDK 
#
# This module defines
# ASSIMP_FOUND       - Assimp was found
# ASSIMP_INCLUDE_DIR - Directory containing ASSIMP header files
# ASSIMP_LIBRARY     - Library name of Assimp library

# Don't be verbose if previously run successfully
IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
   SET(ASSIMP_FIND_QUIETLY TRUE)
ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)

# Set locations to search
IF(UNIX)
   SET(ASSIMP_INCLUDE_SEARCH_DIRS
      /usr/include
      /usr/local/include
	  /usr/local/Cellar/
      /opt/include
      ~/usr/include
      ~/.local/include INTERNAL)
   SET(ASSIMP_LIBRARY_SEARCH_DIRS
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
	  /usr/local/Cellar
      ~/usr/lib
      ~/usr/lib64
      ~/.local/lib INTERNAL)
   SET(ASSIMP_INC_DIR_SUFFIXES PATH_SUFFIXES assimp)
ELSE(UNIX)
   # WIN32
   SET(ASSIMP_INC_DIR_SUFFIXES PATH_SUFFIXES include)
   if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
       set(_LIB_DIR "x64")
   else()
       set(_LIB_DIR "x86")
   endif()
   SET(ASSIMP_LIB_DIR_SUFFIXES PATH_SUFFIXES lib/${_LIB_DIR})
ENDIF(UNIX)

# Set name of the Assimp library to use
IF(UNIX)
   IF(APPLE)
	   SET(ASSIMP_LIBRARY_NAME libassimp.dylib)
   ELSE(APPLE)
      SET(ASSIMP_LIBRARY_NAME libassimp.so)
   ENDIF(APPLE)
ELSE(UNIX)
   # TODO Different libraries are provided for compile and runtime
   SET(ASSIMP_LIBRARY_NAME assimp.lib)
ENDIF(UNIX)

# Search for header files
FIND_PATH(ASSIMP_INCLUDE_DIR Importer.hpp
    PATHS ${ASSIMP_INCLUDE_SEARCH_DIRS} ${ASSIMP_INCLUDE_SEARCH_PATHS} ${ASSIMP_ROOT}
    PATH_SUFFIXES ${ASSIMP_INC_DIR_SUFFIXES})

# Search for library
FIND_LIBRARY(ASSIMP_LIBRARY ${ASSIMP_LIBRARY_NAME}
    PATHS ${ASSIMP_LIBRARY_SEARCH_DIRS} ${ASSIMP_INCLUDE_SEARCH_PATHS} ${ASSIMP_ROOT}
    PATH_SUFFIXES ${ASSIMP_LIB_DIR_SUFFIXES})

SET(ASSIMP_INCLUDE_DIR ${ASSIMP_INCLUDE_DIR} CACHE STRING
    "Directory containing ASSIMP header files")
SET(ASSIMP_LIBRARY ${ASSIMP_LIBRARY} CACHE STRING "Library name of Assimp library")

INCLUDE ( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Assimp
    DEFAULT_MSG ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR)
MARK_AS_ADVANCED(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY)
