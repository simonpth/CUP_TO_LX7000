cmake_minimum_required(VERSION 3.16)

project(CUP_TO_LX7000 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Positioning Core)

qt_standard_project_setup(REQUIRES 6.8)

#icons
set(MACOSX_BUNDLE_ICON_FILE CUPLX7000AppIcon.icns)

set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/appIcon/CUPLX7000AppIcon.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")

qt_add_executable(appCUP_TO_LX7000 MACOSX_BUNDLE
    main.cpp
    ${app_icon_resource_windows}
    ${app_icon_macos}
)

qt_add_qml_module(appCUP_TO_LX7000
    URI CUP_TO_LX7000
    QML_FILES
        Main.qml
        SOURCES airport.h airport.cpp
        SOURCES
        QML_FILES AirportMap.qml
        QML_FILES AirportImportView.qml
        QML_FILES SettingsView.qml
        QML_FILES AirportEditDialog.qml
        QML_FILES AirportSearchView.qml
        SOURCES airportlistmodel.h airportlistmodel.cpp
        SOURCES airportcontroller.h airportcontroller.cpp
        SOURCES airportsortfiltermodel.h airportsortfiltermodel.cpp
        SOURCES airporthelper.h airporthelper.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCUP_TO_LX7000 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCUP_TO_LX7000
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appCUP_TO_LX7000
    PRIVATE
        Qt6::Quick
        Qt6::Positioning
        Qt6::Core
)
target_link_libraries(appCUP_TO_LX7000 PRIVATE Qt6::Core)
target_link_libraries(appCUP_TO_LX7000 PRIVATE Qt6::Core)
target_link_libraries(appCUP_TO_LX7000 PRIVATE Qt6::Core)

include(GNUInstallDirs)
install(TARGETS appCUP_TO_LX7000
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
