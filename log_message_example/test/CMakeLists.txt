set(PROJ_NAME "remote-log-test")
project(${PROJ_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")

# Tests without testing framework
set (NOFRAMEWORK_TARGET noframework-${PROJ_NAME})

include(CTest)

include(FetchContent)

# For command line parsing
FetchContent_Declare(cxxopts 
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git 
    GIT_TAG master)
FetchContent_MakeAvailable(cxxopts)

enable_testing()

add_executable(${NOFRAMEWORK_TARGET} noframework.cpp)

add_custom_command(TARGET ${NOFRAMEWORK_TARGET} POST_BUILD  
                 COMMAND ${CMAKE_COMMAND} -E copy 
                 "${CMAKE_BINARY_DIR}/test/CTestTestfile.cmake" 
                 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

target_include_directories(${NOFRAMEWORK_TARGET} PUBLIC 
    "${CMAKE_BINARY_DIR}/_deps/cxxopts-src/include")
target_include_directories(${NOFRAMEWORK_TARGET} PUBLIC 
    "${CMAKE_SOURCE_DIR}/src")

add_test(NAME NoArguments COMMAND ${NOFRAMEWORK_TARGET} -n)
add_test(NAME EmptyMessage COMMAND ${NOFRAMEWORK_TARGET} -e)

# Tests with googletest
set(GTEST_TARGET gtest-${PROJ_NAME})
add_executable(${GTEST_TARGET} googletest.cpp)

# Fetch googletest
FetchContent_Declare(googletest 
    GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG main)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON CACHE BOOL "" FORCE)

add_custom_command(TARGET ${GTEST_TARGET} POST_BUILD  
                 COMMAND ${CMAKE_COMMAND} -E copy 
                 "${CMAKE_BINARY_DIR}/test/CTestTestfile.cmake" 
                 "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

target_include_directories(${GTEST_TARGET} PUBLIC "${CMAKE_HOME_DIRECTORY}/src")
target_include_directories(${GTEST_TARGET} 
    PUBLIC "${CMAKE_BINARY_DIR}/_deps/googletest-src/include")
target_link_libraries(${GTEST_TARGET} gtest gtest_main)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)
gtest_discover_tests(${GTEST_TARGET})

# Tests with Catch2
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

include(Catch)
catch_discover_tests(${CATCH2_TARGET})
