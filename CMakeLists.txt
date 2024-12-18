cmake_minimum_required(VERSION 3.10)
project(GTK VERSION 0.1.0 LANGUAGES C CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

add_executable(GTK main.cpp)

target_link_libraries(GTK ${GTK_LIBRARIES})
target_include_directories(GTK PRIVATE ${GTK_INCLUDE_DIRS})

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)