cmake_minimum_required(VERSION 3.10.0)
project(vip_lang VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

include_directories(${PROJECT_SOURCE_DIR})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/**/*)

add_executable(vip_lang src/main.cpp ${SRC_FILES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
