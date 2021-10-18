include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include("${PROJECT_SOURCE_DIR}/workstation/app/src.cmake")
include("${PROJECT_SOURCE_DIR}/workstation/base/src.cmake")
include("${PROJECT_SOURCE_DIR}/workstation/scene/src.cmake")
include("${PROJECT_SOURCE_DIR}/workstation/io/src.cmake")

include_directories(${dcmtk_include})
include_directories(${itk_include})
link_directories("${itk_lib}")

file(GLOB itk_libsg ${itk_lib}/debug/*.lib)
file(GLOB itk_libsr ${itk_lib}/release/*.lib)


add_executable(VTKImageTool ${src}  ${PROJECT_SOURCE_DIR}/workstation/gui/MainWindow.qrc)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(VTK REQUIRED)

# find_package(GTest REQUIRED)
# find_package(ITK CONFIG REQUIRED)
# include(${ITK_USE_FILE})

#include(${VTK_USE_FILE})
# message("-*-*-*-${ITK_USE_FILE}")
# if (ITK_FOUND)
#    include(${ITK_USE_FILE})
# else(ITK_FOUND)
#    message(FATAL_ERROR "Cannot build without ITK.  Please set ITK_DIR.")
# endif(ITK_FOUND)
# include_directories(${itk_include})
# file(GLOB itk_libdeg ${itk_lib}/Debug/*.lib)
# file(GLOB itk_librel ${itk_lib}/Release/*.lib)

# set(itk_dug_list)
# foreach(tmp ${itk_libdeg})
#         set(itk_dug_list ${itk_dug_list} ${tmp})
# endforeach()
# message("deb:${itk_dug_list}")

# set(itk_rel_list)
# foreach(tmp ${itk_librel})
#         set(itk_rel_list ${itk_rel_list} ${tmp})
# endforeach()
# message("***rel:${itk_rel_list}")


target_link_libraries(VTKImageTool
        PRIVATE ${VTK_LIBRARIES}
        PRIVATE spdlog::spdlog spdlog::spdlog_header_only
        # PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main
        # ${ITK_LIBRARIES}
        # PRIVATE ITKFFT ITKVTK ITKznz itksys itkvcl
        debug ${itk_libsg}
        # optimized ${itk_libsr}
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        debug ${dcmtk_lib}/debug/dcmtk.lib
        optimized ${dcmtk_lib}/release/dcmtk.lib
        # debug ${itk_dug_list}
        # optimized ${itk_rel_list}
)

#dctmtk
file(COPY "${dcmtk_bin}/debug/dcmtk.dll" DESTINATION "${PROJECT_OUTPUT_DIR}/debug")
file(COPY "${dcmtk_bin}/release/dcmtk.dll" DESTINATION "${PROJECT_OUTPUT_DIR}/release")
message("***${itk_bin}")
configure_files("${itk_bin}/debug" "${PROJECT_OUTPUT_DIR}/debug")
configure_files("${itk_bin}/release" "${PROJECT_OUTPUT_DIR}/release")

configure_files("${PROJECT_SOURCE_DIR}/workstation/gui/images" "${PROJECT_OUTPUT_DIR}/debug/images")
configure_files("${PROJECT_SOURCE_DIR}/workstation/gui/images" "${PROJECT_OUTPUT_DIR}/release/images")
configure_files("${PROJECT_SOURCE_DIR}/workstation/gui/Resources" "${PROJECT_OUTPUT_DIR}/debug/Resources")
configure_files("${PROJECT_SOURCE_DIR}/workstation/gui/Resources" "${PROJECT_OUTPUT_DIR}/release/Resources")


# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Cored.dll DESTINATION ${PROJECT_OUTPUT_DIR}/debug/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Core.dll DESTINATION ${PROJECT_OUTPUT_DIR}/release/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Widgetsd.dll DESTINATION ${PROJECT_OUTPUT_DIR}/debug/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Widgets.dll DESTINATION ${PROJECT_OUTPUT_DIR}/release/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Guid.dll DESTINATION ${PROJECT_OUTPUT_DIR}/debug/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Gui.dll DESTINATION ${PROJECT_OUTPUT_DIR}/release/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5PrintSupportd.dll DESTINATION ${PROJECT_OUTPUT_DIR}/debug/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5PrintSupport.dll DESTINATION ${PROJECT_OUTPUT_DIR}/release/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Networkd.dll DESTINATION ${PROJECT_OUTPUT_DIR}/debug/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Network.dll DESTINATION ${PROJECT_OUTPUT_DIR}/release/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Qmld.dll DESTINATION ${PROJECT_OUTPUT_DIR}/debug/)
# FILE(COPY ${_qt5Widgets_install_prefix}/bin/Qt5Qml.dll DESTINATION ${PROJECT_OUTPUT_DIR}/release/)