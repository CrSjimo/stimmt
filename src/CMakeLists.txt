set(STIMMT_PROJECT_DESCRIPTION "Script Task Integrated Manager for Musical Tools")
set(STIMMT_PROJECT_COPYRIGHT "Copyright 2023 CrSjimo")
set(STIMMT_GENERATED_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/../include)

# ----------------------------------
# CMake API
# ----------------------------------
function(stimmt_add_library _target)
    set(options)
    set(oneValueArgs PREFIX)
    set(multiValueArgs)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(CMAKE_AUTOUIC on)
    set(CMAKE_AUTOMOC on)
    set(CMAKE_AUTORCC on)

    if(STIMMT_BUILD_STATIC)
        set(_type STATIC)
    else()
        set(_type SHARED)
    endif()

    add_library(${_target} ${_type})

    if(WIN32 AND NOT STIMMT_BUILD_STATIC)
        qtmediate_add_win_rc(${_target}
            DESCRIPTION ${STIMMT_PROJECT_DESCRIPTION}
            COPYRIGHT ${STIMMT_PROJECT_COPYRIGHT}
        )
    endif()

    if(FUNC_PREFIX)
        set(_prefix_option PREFIX ${FUNC_PREFIX})
    else()
        set(_prefix_option)
    endif()

    # Set global definitions
    qtmediate_export_defines(${_target} ${_prefix_option})

    # Configure target
    qtmediate_configure_target(${_target} ${FUNC_UNPARSED_ARGUMENTS})

    if(STIMMT_INSTALL)
        install(TARGETS ${_target}
            EXPORT ${STIMMT_INSTALL_NAME}Targets
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" OPTIONAL
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" OPTIONAL
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" OPTIONAL
        )

        target_include_directories(${PROJECT_NAME} PUBLIC
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${STIMMT_INSTALL_NAME}>"
        )

        set(_gen_include_options INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/${STIMMT_INSTALL_NAME}/${_target}")
    else()
        set(_gen_include_options)
    endif()

    # Generate a standard include directory in build directory
    qtmediate_sync_include(. "${STIMMT_GENERATED_INCLUDE_DIR}/${_target}" ${_gen_include_options})
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$<BUILD_INTERFACE:${STIMMT_GENERATED_INCLUDE_DIR}>"
    )

    target_include_directories(${PROJECT_NAME} PRIVATE .)

    # Add to translate targets
    set_property(GLOBAL APPEND PROPERTY STIMMT_TRANSLATE_TARGETS ${_target})
endfunction()

# ----------------------------------
# Main Project
# ----------------------------------
set(CMAKE_AUTOUIC on)
set(CMAKE_AUTOMOC on)
set(CMAKE_AUTORCC on)

file(GLOB_RECURSE _src *.h *.cpp *.qrc)

stimmt_add_library(${PROJECT_NAME}
    SOURCES ${_src}
#    LINKS QMCore
    # TODO links QtMediate
    QT_LINKS Core Gui Widgets Qml Xml Svg
    QT_INCLUDE_PRIVATE Core Gui Widgets Qml Xml Svg
)

# ----------------------------------
# Documentation
# ----------------------------------
if(STIMMT_BUILD_DOCUMENTATIONS)
    if(NOT DOXYGEN_EXECUTABLE)
        find_package(Doxygen REQUIRED)
    endif()

    set(_install_options)

    if(STIMMT_INSTALL)
        set(_install_options INSTALL_DIR share/doc/${STIMMT_INSTALL_NAME})
    endif()

    set(STIMMT_DOXYGEN_TAGFILE ${CMAKE_BUILD_SHARE_DIR}/doc/${STIMMT_INSTALL_NAME}/stimmt_tagfile.xml
        CACHE FILEPATH "stimmt doxygen tag file path" FORCE
    )

    set(_doc_targets StimmtCore StimmtDevice StimmtFormat StimmtRemote StimmtSynthesis)

    qtmediate_setup_doxygen(stimmt_RunDoxygen
        NAME "STIMMT"
        DESCRIPTION "Script Task Integrated Manager for Musical Tools"
        MDFILE ../README.md
        OUTPUT_DIR ${CMAKE_BUILD_SHARE_DIR}/doc/${STIMMT_INSTALL_NAME}
        INPUT .
        TARGETS ${_doc_targets}
        DEPENDS ${_doc_targets}
        NO_EXPAND_MACROS Q_OBJECT Q_GADGET Q_DECLARE_TR_FUNCTIONS
        COMPILE_DEFINITIONS Q_SIGNALS=Q_SIGNALS Q_SLOTS=Q_SLOTS
        GENERATE_TAGFILE "${STIMMT_DOXYGEN_TAGFILE}"
        ${_install_options}
    )
endif()

if(STIMMT_INSTALL)
    # Add install target
    set(_install_dir ${CMAKE_INSTALL_LIBDIR}/cmake/${STIMMT_INSTALL_NAME})

    # Add version file
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${STIMMT_INSTALL_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
    )

    # Add configuration file
    configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/${STIMMT_INSTALL_NAME}Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${STIMMT_INSTALL_NAME}Config.cmake"
        INSTALL_DESTINATION ${_install_dir}
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    # Install cmake files
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${STIMMT_INSTALL_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${STIMMT_INSTALL_NAME}ConfigVersion.cmake"
        DESTINATION ${_install_dir}
    )

    # Install cmake targets files
    install(EXPORT ${STIMMT_INSTALL_NAME}Targets
        FILE "${STIMMT_INSTALL_NAME}Targets.cmake"
        NAMESPACE stimmt::
        DESTINATION ${_install_dir}
    )
endif()