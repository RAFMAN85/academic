cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(PNG)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
main.cpp
dialog.cpp
dialog.h
dialog.ui
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)


set_target_properties(${PROJECT_NAME} PROPERTIES
CXX_EXTENSIONS OFF
AUTOMOC ON
AUTOUIC ON
AUTORCC ON
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(png IMPORTED_TARGET REQUIRED libpng16)
target_link_libraries(${PROJECT_NAME} PRIVATE PkgConfig::png)
