#Theme::QT5 + VCPKG + cmake
#Time:: 2021-08-05
#Author::gcl
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
cmake_policy(SET CMP0079 NEW)
# vcpkg
set(CMAKE_TOOLCHAIN_FILE "../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(VTKImageTool)
include (FindOpenMP)

set_property(GLOBAL PROPERTY USE_FOLDERS ON) # /cmake for support folders
if (OPENMP_FOUND)
  message("build with openmp")
  set (HAVE_OPENMP TRUE)
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif()

include("${PROJECT_SOURCE_DIR}/macro.cmake")
# include("${PROJECT_SOURCE_DIR}/cmake/Config/GitConfig.cmake")
# include("${PROJECT_SOURCE_DIR}/cmake/Config/OpenMPConfig.cmake")
# include("${PROJECT_SOURCE_DIR}/cmake/Config/PlatformConfig.cmake")
# include("${PROJECT_SOURCE_DIR}/cmake/Config/UtilsConfig.cmake")

#build setting: just unicode version
add_definitions(-D_BIND_TO_CURRENT_VCLIBS_VERSION=1)
add_definitions(-D_UNICODE -DUNICODE)
add_definitions(-DPROTOBUF_USE_DLLS)
#set project output directory
string(REGEX REPLACE "/code" "" PROJECT_OUTPUT_DIR ${PROJECT_SOURCE_DIR})
set(PROJECT_UP_DIR "${PROJECT_OUTPUT_DIR}")
set(PROJECT_OUTPUT_DIR "${PROJECT_OUTPUT_DIR}/output")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_OUTPUT_DIR}")
set(LIBRARY_OUTPUT_PATH "${PROJECT_OUTPUT_DIR}")

option (CHECK_UPDATE_FOR_SOFTWARE_MACRO "UPDATE FOR SOFTWARE" ON)
option (GENERATE_DEBUG_INFO "always generate debug info" OFF)
# if(CHECK_UPDATE_FOR_SOFTWARE_MACRO)
# add_definitions(-DCHECK_UPDATE_FOR_SOFTWARE_MACRO)
# endif()

SET(CMAKE_DEBUG_POSTFIX d)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc") # /EHsc for support c++ exception

# utils
set(UTILS_PATH "${PROJECT_UP_DIR}/utils")
# find_path(UTILS_PATH NAMES)
message(${UTILS_PATH})
include(${UTILS_PATH}/utils_path.cmake)

find_package(spdlog CONFIG REQUIRED)
include_directories(${spdlog_include})

# include_directories(${BOOST_PATH})
# if((MSVC_VERSION GREATER 1919) AND (MSVC_VERSION LESS 1930))
#   configure_files_selected("${BOOST_PATH}/lib64-msvc-14.2/" "*mt-gd-*.dll" 	"${PROJECT_OUTPUT_DIR}/debug")
#   configure_files_selected("${BOOST_PATH}/lib64-msvc-14.2/" "*mt-*.dll" 	"${PROJECT_OUTPUT_DIR}/release")
#   link_directories(${BOOST_PATH}/lib64-msvc-14.2)
# elseif((MSVC_VERSION GREATER 1909) AND (MSVC_VERSION LESS 1920))
#   configure_files_selected("${BOOST_PATH}/lib64-msvc-14.1/" "*mt-gd-*.dll" 	"${PROJECT_OUTPUT_DIR}/debug")
#   configure_files_selected("${BOOST_PATH}/lib64-msvc-14.1/" "*mt-*.dll" 	"${PROJECT_OUTPUT_DIR}/release")
#   link_directories(${BOOST_PATH}/lib64-msvc-14.1)
# endif()

set (CMAKE_SKIP_RULE_DEPENDENCY TRUE)
subdirs(workstation)




