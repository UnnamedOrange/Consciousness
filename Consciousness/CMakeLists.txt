cmake_minimum_required(VERSION 3.5)

project(
    Consciousness
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/i18n/Consciousness_zh_CN.ts")

file(
    GLOB_RECURSE
    PROJECT_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.ui"
)

set(PROJECT_SOURCES "${PROJECT_SOURCES}" "${TS_FILES}")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})
    # Define target properties for Android with Qt 6 as: set_property(TARGET ${PROJECT_NAME} APPEND
    # PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android) For more
    # information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_add_lupdate(${PROJECT_NAME} TS_FILES ${TS_FILES})
    qt_add_lrelease(${PROJECT_NAME} TS_FILES ${TS_FILES} QM_FILES_OUTPUT_VARIABLE QM_FILES)
    qt_add_resources(
        ${PROJECT_NAME}
        "translations"
        PREFIX
        "/i18n"
        BASE
        "${CMAKE_CURRENT_BINARY_DIR}"
        FILES
        "${QM_FILES}"
    )
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
        # Define properties for Android with Qt 5 after find_package() calls as:
        # set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
    endif()

    qt5_create_translation(QM_FILES "${CMAKE_SOURCE_DIR}" "${TS_FILES}")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER UnnamedOrange.Consciousness.com
               MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
               MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               MACOSX_BUNDLE TRUE
               WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
