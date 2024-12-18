cmake_minimum_required(VERSION 3.10)
project(OAL VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenAL REQUIRED)

include_directories(${OPENAL_INCLUDE_DIRS})

add_executable(OAL main.cpp)

target_link_libraries(OAL ${OPENAL_LIBRARY}) 

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)