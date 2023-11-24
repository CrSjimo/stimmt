cmake_minimum_required(VERSION 3.17)

project(stimmt VERSION 0.0.1.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# ----------------------------------
# Build Options
# ----------------------------------
option(STIMMT_BUILD_STATIC "Build static libraries" OFF)
option(STIMMT_BUILD_TRANSLATIONS "Build translations" ON)
option(STIMMT_BUILD_TESTS "Build test cases" ON)
option(STIMMT_BUILD_DOCUMENTATIONS "Build documentations" OFF)
option(STIMMT_INSTALL "Install library" ON)

# ----------------------------------
# CMake Settings
# ----------------------------------
if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

if(NOT DEFINED CMAKE_BUILD_SHARE_DIR)
    set(CMAKE_BUILD_SHARE_DIR ${CMAKE_BINARY_DIR}/share)
endif()

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /manifest:no")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

    if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
        set(CMAKE_DEBUG_POSTFIX "d")
    endif()
endif()

if(STIMMT_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)
endif()

# ----------------------------------
# Project Variables
# ----------------------------------
set(STIMMT_MODULES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(STIMMT_VERSION ${PROJECT_VERSION})
set(STIMMT_INSTALL_NAME ${PROJECT_NAME})
set(STIMMT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(STIMMT_TRANSLATIONS zh_CN zh_TW ja_JP)

# ----------------------------------
# Main Project
# ----------------------------------
find_package(qtmediate-cmake-modules REQUIRED)

add_subdirectory(src)
add_subdirectory(share)

if(STIMMT_BUILD_TESTS)
    add_subdirectory(tests)
endif()