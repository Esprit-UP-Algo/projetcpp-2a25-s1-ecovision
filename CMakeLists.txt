cmake_minimum_required(VERSION 3.16)

project(eco_vision VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Find Qt packages ----
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# ---- Add project sources ----
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    connection.cpp
    connection.h
    resource.cpp
    resource.h
    image.qrc
)

# ---- Create the executable ----
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(eco_vision
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(eco_vision
        ${PROJECT_SOURCES}
    )
endif()

# ---- Link libraries ----
target_link_libraries(eco_vision PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# ---- App properties ----
set_target_properties(eco_vision PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# ---- Installation (optional) ----
include(GNUInstallDirs)
install(TARGETS eco_vision
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# ---- Finalize executable for Qt 6 ----
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(eco_vision)
endif()
