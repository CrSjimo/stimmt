if(STIMMT_BUILD_TRANSLATIONS)
    qm_import(Translate)
    find_package(Qt5 COMPONENTS LinguistTools)

    qm_add_translation(${PROJECT_NAME}_translations
        LOCALES ${STIMMT_TRANSLATIONS}
        TARGETS ${PROJECT_NAME}
        TS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/translations
        QM_DIR ${CMAKE_CURRENT_BINARY_DIR}/translations
    )
endif()