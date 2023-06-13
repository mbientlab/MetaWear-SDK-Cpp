#----------------
# Setup
#----------------

include(cmake/ReadVariables.cmake)
cmake_minimum_required(VERSION 3.21)
project(metawear)

# Use the following from subdirectories as required
# CAUTION: this sets variables in the current scope (directory/function)
ReadVariables(project_version.mk)

if(DEFINED CXX_VERSION STREQUAL 14)
	set(CMAKE_CXX_STANDARD 14)
	message(STATUS "SETUP: CXX=14")
elseif(DEFINED CXX_VERSION STREQUAL 17)
	set(CMAKE_CXX_STANDARD 17)
	message(STATUS "SETUP: CXX=17")
else()
	set(CMAKE_CXX_STANDARD 11)
	message(STATUS "SETUP: CXX=11")
endif()

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
   message(STATUS "Build type not specified: default is Release")
endif()

message(STATUS "CMAKE_VERSION=${CMAKE_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Project Version: ${VERSION}")

if(MSVC OR WIN32)
	# enable MSVC_RUNTIME_LIBRARY target property
	# see https://cmake.org/cmake/help/latest/policy/CMP0091.html
	if(POLICY CMP0091)
  		cmake_policy(SET CMP0091 NEW)
	endif()
endif()

# Prepend our CMake modules directory
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_MODULE_PATH='${CMAKE_MODULE_PATH}'")

#----------------
# Sources
#----------------

# collect and add source files
set(MODULES "metawear/platform" "metawear/core" "metawear/peripheral" "metawear/processor" "metawear/sensor" "metawear/impl" "metawear/dfu")
set(SOURCE_DIR "src")
set(BUILD_DIR "build")
set(DIST_DIR "dist")

# include directory
include_directories(${SOURCE_DIR})

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
    set(DIST_PATH "${DIST_DIR}/x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
    set(DIST_PATH "${DIST_DIR}/x64_86")
endif()

foreach(MODULE ${MODULES})
	file(GLOB SOURCE_GLOB "${SOURCE_DIR}/${MODULE}/cpp/*.cpp")
	set(SOURCES ${SOURCES} ${SOURCE_GLOB})
	file(GLOB HEADER_GLOB "${SOURCE_DIR}/${MODULE}/cpp/*.h")
	set(HEADERS ${HEADERS} ${HEADER_GLOB})
	file(GLOB EXPORT_HEADER_GLOB "${SOURCE_DIR}/${MODULE}/*.h")
	set(EXPORT_HEADERS ${EXPORT_HEADERS} ${EXPORT_HEADER_GLOB})
endforeach()
# message(STATUS ${SOURCES})
# message(STATUS ${HEADERS})

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

#----------------
# Compiler flags
#----------------

set(COMMON_FLAGS "-std=c++11 -fPIC -fvisibility=hidden -fvisibility-inlines-hidden -Wall -Werror -I${SOURCE_DIR} -DMETAWEAR_DLL -DMETAWEAR_DLL_EXPORTS ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
set(CMAKE_C_FLAGS ${COMMON_FLAGS})

message(STATUS "CMAKE_CXX_COMPILER_VERSION='${CMAKE_CXX_COMPILER_VERSION}'")
message(STATUS "CMAKE_CXX_FLAGS='${CMAKE_CXX_FLAGS}'")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG='${CMAKE_CXX_FLAGS_DEBUG}'")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE='${CMAKE_CXX_FLAGS_RELEASE}'")
message(STATUS "CMAKE_CXX_COMPILE_OBJECT='${CMAKE_CXX_COMPILE_OBJECT}'")

if(MSVC OR WIN32)
	if(COMPILE_MT)
    	add_compile_options(
        	$<$<CONFIG:>:/MT>
        	$<$<CONFIG:Debug>:/MTd>
        	$<$<CONFIG:Release>:/MT>
    	)
    	set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
	endif()
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${VERSION}
    EXPORT_NAME ${PROJECT_NAME}
    OUTPUT_NAME ${PROJECT_NAME}
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${DIST_PATH}/${CMAKE_BUILD_TYPE})
message(STATUS "Output Dir: ${PROJECT_BINARY_DIR}/${DIST_PATH}/${CMAKE_BUILD_TYPE}")

# Configure include directories
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set_property(TARGET ${PROJECT_NAME} PROPERTY INSTALL_RPATH $ORIGIN)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set_property(TARGET ${PROJECT_NAME} PROPERTY INSTALL_RPATH @loader_path)
endif()

#----------------
# Install
#----------------

install(TARGETS ${PROJECT_NAME} DESTINATION "${PROJECT_BINARY_DIR}/${DIST_PATH}/${CMAKE_BUILD_TYPE}")

install(
    TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

#-------------------------------------------------------------------------------