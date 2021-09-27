#Theme::QT5 + VCPKG + cmake
#Time:: 2021-08-05
#Author::gcl
cmake_minimum_required(VERSION 3.3 FATAL_ERROR)
set(CMAKE_TOOLCHAIN_FILE "../vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

project(VTKImageTool)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") 
# #set project output directory
# string(REGEX REPLACE "/code" "" PROJECT_OUTPUT_DIR ${PROJECT_SOURCE_DIR})
# set(PROJECT_OUTPUT_DIR "${PROJECT_OUTPUT_DIR}/output")
# set(EXECUTABLE_OUTPUT_PATH "${PROJECT_OUTPUT_DIR}")
# set(LIBRARY_OUTPUT_PATH "${PROJECT_OUTPUT_DIR}")

set(src "BasicDefine.h"
        "BasicVtkDefine.h"
        "ViewType.h"
        "MultiImageView.h"
        "MultiImageView.cpp"
        "BasicImageStyle.h"
        "BasicImageStyle.cpp"
        "QVTKGraphView.h"
        "QVTKGraphView.cpp"
        "mainWindow.h"
        "mainWindow.cpp"
        "MultiImageView2.h"
        "MultiImageView2.cpp"
        "MPRStyle.h"
        "MPRStyle.cpp"
        "MousePicker.h"
        "MousePicker.cpp"
        "SceneReslice.h"
        "SceneReslice.cpp"
        "SceneCrossLine.h"
        "SceneCrossLine.cpp"
        "ViewRenderScene.h"
        "ViewRenderScene.cpp"
        "json.hpp"
        "MainWindow.ui" )

add_executable(VTKImageTool MACOSX_BUNDLE main.cxx ${src})

find_package(spdlog CONFIG REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
target_link_libraries(VTKImageTool 
        PRIVATE ${VTK_LIBRARIES}
        PRIVATE spdlog::spdlog spdlog::spdlog_header_only
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
  )

  # FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Cored.dll DESTINATION ${PROJECT_OUTPUT_DIR}/debug/)
  # FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Widgetsd.dll DESTINATION ${PROJECT_OUTPUT_DIR}/debug/)
  # FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Guid.dll DESTINATION ${PROJECT_OUTPUT_DIR}/debug/)
  # FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Core.dll DESTINATION ${PROJECT_OUTPUT_DIR}/release/)
  # FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Widgets.dll DESTINATION ${PROJECT_OUTPUT_DIR}/release/)
  # FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Gui.dll DESTINATION ${PROJECT_OUTPUT_DIR}/release/)