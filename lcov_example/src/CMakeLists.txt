set(PROJ_NAME "systemundertest")
project(${PROJ_NAME})

add_library(${PROJ_NAME} STATIC system_under_test.cpp)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_options(${PROJ_NAME} PRIVATE --coverage)
    target_link_options(${PROJ_NAME} PUBLIC --coverage)
    add_custom_command(TARGET ${PROJ_NAME} PRE_BUILD COMMAND
    find ${CMAKE_BINARY_DIR} -type f -name '*.gcda' -exec rm {} +)
endif()

add_executable(${PROJ_NAME}-runner main.cpp)
target_link_libraries(${PROJ_NAME}-runner PRIVATE ${PROJ_NAME})