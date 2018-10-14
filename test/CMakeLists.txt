cmake_minimum_required(VERSION 3.10)


if (APPLE)
	project(googletest-download NONE)

	include(ExternalProject)

	#ExternalProject_Add(googletest
	#    GIT_REPOSITORY https://github.com/google/googletest.git
	#    GIT_TAG master
	#    SOURCE_DIR "${CMAKE_BINARY_DIR}/googletest-src"
	#    BINARY_DIR "${CMAKE_BINARY_DIR}/googletest-build"
	#    CONFIGURE_COMMAND ""
	#    BUILD_COMMAND ""
	#    INSTALL_COMMAND ""
	#    TEST_COMMAND ""
	#)


	ExternalProject_Add(googletest
		GIT_REPOSITORY https://github.com/google/googletest.git
		GIT_TAG master
		CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:Path=Debug
		CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:Path=Release
		-DBUILD_GTEST=ON
		-DCMAKE_CXX_STANDARD=14
		PREFIX "${CMAKE_CURRENT_BINARY_DIR}/gtest"
		INSTALL_COMMAND ""
	)


	ExternalProject_Get_Property(googletest source_dir)
	set (GTEST_INCLUDE_DIR ${source_dir}/googletest/include)


	ExternalProject_Get_Property(googletest binary_dir)
	set (GTEST_LIBS_DIR ${binary_dir}/googlemock/gtest)


	enable_testing()
	set (LOCAL_PROJECT_1_TEST ${LOCAL_PROJECT_1}_test)

	message (STATUS ${CMAKE_CURRENT_SOURCE_DIR})
	set ( MY_LOCAL_HEADER_FILES_PROJECT_1_TEST
		${CMAKE_SOURCE_DIR}/src/Vector.hpp
		${CMAKE_SOURCE_DIR}/src/Matrix.hpp
		${CMAKE_SOURCE_DIR}/src/Algebra.hpp
		${CMAKE_SOURCE_DIR}/src/Degree.hpp
		${CMAKE_SOURCE_DIR}/src/Radian.hpp
		${CMAKE_SOURCE_DIR}/src/Angle.hpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/UnitTestHelperFunctions.hpp
	)

 
	set ( MY_LOCAL_SOURCE_FILES_PROJECT_1_TEST
		${CMAKE_SOURCE_DIR}/test/unit\ test/UnitTest.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Vector2.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Vector3.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Vector4.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Matrix4x4.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Quaternion.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Degree.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Radian.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/UnitTestHelperFunctions.cpp
	)   


	add_executable (${LOCAL_PROJECT_1_TEST} ${MY_LOCAL_SOURCE_FILES_PROJECT_1_TEST} ${MY_LOCAL_HEADER_FILES_PROJECT_1_TEST})
	target_include_directories (${LOCAL_PROJECT_1_TEST} PUBLIC ${GTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src)
	target_link_libraries (${LOCAL_PROJECT_1_TEST} debug ${GTEST_LIBS_DIR}/Debug/libgtestd.a)
	target_link_libraries (${LOCAL_PROJECT_1_TEST} optimized ${GTEST_LIBS_DIR}/Debug/libgtest.a)
	add_dependencies (${LOCAL_PROJECT_1_TEST} googletest)

elseif (WIN32)
	set (LOCAL_PROJECT_1_TEST ${LOCAL_PROJECT_1}_test)


	# Define the name of the project
	project (${LOCAL_PROJECT_1_TEST})


	set ( MY_LOCAL_HEADER_FILES_PROJECT_1_TEST
			${CMAKE_SOURCE_DIR}/src/Vector.hpp
			${CMAKE_SOURCE_DIR}/src/Matrix.hpp
			${CMAKE_SOURCE_DIR}/src/Algebra.hpp
			${CMAKE_SOURCE_DIR}/src/Degree.hpp
			${CMAKE_SOURCE_DIR}/src/Radian.hpp
			${CMAKE_SOURCE_DIR}/src/Angle.hpp
			${CMAKE_SOURCE_DIR}/test/unit\ test/UnitTestHelperFunctions.hpp
	)

 
	set ( MY_LOCAL_SOURCE_FILES_PROJECT_1_TEST
		${CMAKE_SOURCE_DIR}/test/unit\ test/UnitTest.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Vector2.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Vector3.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Vector4.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Matrix4x4.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Quaternion.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Degree.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/Radian.test.cpp
		${CMAKE_SOURCE_DIR}/test/unit\ test/UnitTestHelperFunctions.cpp
	)
   

	set (GTEST_INCLUDE_DIR CACHE PATH "Google test include directory")
	set (GTEST_LIBRARY CACHE FILEPATH "Google test library")

	add_executable (${LOCAL_PROJECT_1_TEST} ${MY_LOCAL_SOURCE_FILES_PROJECT_1_TEST} ${MY_LOCAL_HEADER_FILES_PROJECT_1_TEST})
	target_include_directories(${LOCAL_PROJECT_1_TEST} PUBLIC ${GTEST_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/src)
	target_compile_definitions (${LOCAL_PROJECT_1_TEST} PUBLIC "_USE_MATH_DEFINES")
	target_link_libraries (${LOCAL_PROJECT_1_TEST} ${GTEST_LIBRARY})

endif()



