cmake_minimum_required(VERSION 3.29)

set(PROJ_NAME "remote-log-test")
project(${PROJ_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

include(CTest)

include(FetchContent)

enable_testing()

set(CATCH2_TARGET catch2-${PROJ_NAME})

add_executable(${CATCH2_TARGET} catch2test.cpp)

# Fetch Catch2
FetchContent_Declare(Catch2 
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git GIT_TAG devel)
FetchContent_MakeAvailable(Catch2)

add_custom_command(TARGET ${CATCH2_TARGET} POST_BUILD  
                 COMMAND ${CMAKE_COMMAND} -E copy 
                 "${CMAKE_BINARY_DIR}/test/CTestTestfile.cmake" 
                 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

target_include_directories(${CATCH2_TARGET} PUBLIC "${CMAKE_HOME_DIRECTORY}/src")
target_link_libraries(${CATCH2_TARGET} PRIVATE Catch2::Catch2WithMain)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    target_compile_options(${CATCH2_TARGET} PRIVATE --coverage)
    target_link_options(${CATCH2_TARGET} PUBLIC --coverage)
    add_custom_command(TARGET ${CATCH2_TARGET} PRE_BUILD COMMAND
    find ${CMAKE_BINARY_DIR} -type f -name '*.gcda' -exec rm {} +)
endif()

include(Coverage)
RunCoverageReport(${CATCH2_TARGET})

include(Catch)
catch_discover_tests(${CATCH2_TARGET})
