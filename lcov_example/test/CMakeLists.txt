set(PROJ_NAME "lcov-example")
project(${PROJ_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

include(FetchContent)

enable_testing()

add_executable(${PROJ_NAME} coverage_test.cpp)

FetchContent_Declare(Catch2 GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG devel)
FetchContent_MakeAvailable(Catch2)

add_custom_command(TARGET ${PROJ_NAME} POST_BUILD  
                 COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/test/CTestTestfile.cmake" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_options(${PROJ_NAME} PRIVATE --coverage)
    target_link_options(${PROJ_NAME} PUBLIC --coverage)
    add_custom_command(TARGET ${PROJ_NAME} PRE_BUILD COMMAND
    find ${CMAKE_BINARY_DIR} -type f -name '*.gcda' -exec rm {} +)
endif()

target_include_directories(${PROJ_NAME} PUBLIC "${CMAKE_HOME_DIRECTORY}/src")
target_link_libraries(${PROJ_NAME} PRIVATE Catch2::Catch2WithMain systemundertest)

include(Coverage)
RunCoverageReport(${PROJ_NAME})

include(Catch)
catch_discover_tests(${PROJ_NAME})
