project(tst_stimmt_QJSEngine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE _src *.h *.cpp)

ck_add_executable(${PROJECT_NAME} ${_src})

ck_target_components(${PROJECT_NAME}
    LINKS stimmt
    QT_LINKS Core Gui Widgets Qml
)