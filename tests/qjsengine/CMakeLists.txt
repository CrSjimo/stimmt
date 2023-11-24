project(tst_stimmt_QJSEngine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE _src *.h *.cpp *.qrc)

add_executable(${PROJECT_NAME} ${_src})

qtmediate_configure_target(${PROJECT_NAME}
    LINKS stimmt
    QT_LINKS Core Gui Widgets Qml
)