# Find ImageMagick
#
# Finds the libraries and header files for the ImageMagick SDK 
#
# This module defines
# IMAGEMAGICK_FOUND       - ImageMagick was found
# IMAGEMAGICK_INCLUDE_DIR - Directory containing ImageMagick header files
# IMAGEMAGICK_LIBRARY     - Library name of ImageMagick library

# Don't be verbose if previously run successfully
IF(IMAGEMAGICK_INCLUDE_DIR AND IMAGEMAGICK_LIBRARY)
   SET(IMAGEMAGICK_FIND_QUIETLY TRUE)
ENDIF(IMAGEMAGICK_INCLUDE_DIR AND IMAGEMAGICK_LIBRARY)

# Set locations to search
IF(UNIX)
   SET(IMAGEMAGICK_INCLUDE_SEARCH_DIRS
      /usr/include
      /usr/local/include
	  /usr/local/Cellar/
      /opt/include
      ~/usr/include
      ~/.local/include INTERNAL)
   SET(IMAGEMAGICK_LIBRARY_SEARCH_DIRS
      /usr/lib
      /usr/lib64
      /usr/local/lib
      /usr/local/lib64
	  /usr/local/Cellar
      ~/usr/lib
      ~/usr/lib64
      ~/.local/lib INTERNAL)
   SET(IMAGEMAGICK_INC_DIR_SUFFIXES PATH_SUFFIXES ImageMagick-6 Magick++ magick wand )
ELSE(UNIX)
   # WIN32
   SET(IMAGEMAGICK_INC_DIR_SUFFIXES PATH_SUFFIXES include)
   if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
       set(_LIB_DIR "x64")
   else()
       set(_LIB_DIR "x86")
   endif()
   SET(IMAGEMAGICK_LIB_DIR_SUFFIXES PATH_SUFFIXES lib)
ENDIF(UNIX)

# Set name of the ImageMagick library to use
IF(UNIX)
   IF(APPLE)
	   SET(IMAGEMAGICK_LIBRARY_NAME libMagick++-6.Q16.dylib libMagickCore-6.Q16.dylib libMagickWand-6.Q16.dylib)
   ELSE(APPLE)
      SET(IMAGEMAGICK_LIBRARY_NAME libImageMagick.so)
   ENDIF(APPLE)
ELSE(UNIX)
   # TODO Different libraries are provided for compile and runtime
   SET(IMAGEMAGICK_LIBRARY_NAME ImageMagick.lib)
ENDIF(UNIX)


# Search for header files
FIND_PATH(IMAGEMAGICK_INCLUDE_DIR Magick++.h 
    PATHS ${IMAGEMAGICK_INCLUDE_SEARCH_DIRS} ${IMAGEMAGICK_INCLUDE_SEARCH_PATHS} ${IMAGEMAGICK_ROOT}
    PATH_SUFFIXES ${IMAGEMAGICK_INC_DIR_SUFFIXES})


# Search for library
FIND_LIBRARY(IMAGEMAGICK_LIBRARY ${IMAGEMAGICK_LIBRARY_NAME}
    PATHS ${IMAGEMAGICK_LIBRARY_SEARCH_DIRS} ${IMAGEMAGICK_INCLUDE_SEARCH_PATHS} ${IMAGEMAGICK_ROOT}
    PATH_SUFFIXES ${IMAGEMAGICK_LIB_DIR_SUFFIXES})


# Set all the paths based on the ones previously found
SET(IMAGEMAGICK_INCLUDE_DIR ${IMAGEMAGICK_INCLUDE_DIR} CACHE STRING "Directory containing IMAGEMAGICK header files")
SET(IMAGEMAGICK_LIBRARY ${IMAGEMAGICK_LIBRARY} CACHE STRING "Library name of ImageMagick library")


INCLUDE ( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ImageMagick DEFAULT_MSG IMAGEMAGICK_LIBRARY IMAGEMAGICK_INCLUDE_DIR)
MARK_AS_ADVANCED(IMAGEMAGICK_INCLUDE_DIR IMAGEMAGICK_LIBRARY)
