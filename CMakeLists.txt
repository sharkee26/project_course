cmake_minimum_required(VERSION 3.10)
project(bd VERSION 0.1.0 LANGUAGES C CXX)

add_library(sqlite3 STATIC lib/sqlite3.c)

add_executable(bd main.cpp)

target_link_libraries(bd sqlite3) 
target_include_directories(bd PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)