cmake_minimum_required(VERSION 3.0.0)

project(flexbox VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Fetch GTest
# Without this code, flexbox_text will not compile properly on linux
include(FetchContent)
include(GoogleTest)
cmake_policy(SET CMP0135 NEW)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

# flexbox
file(GLOB_RECURSE all_h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE all_hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE all_cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${all_h} ${all_hpp} ${all_cpp})
add_library(flexbox ${all_h} ${all_hpp} ${all_cpp})
target_link_libraries(flexbox PUBLIC yogacore)
target_include_directories(flexbox PUBLIC ${VGG_CONTRIB_BOOST_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR}/src)

# flexbox_test
option(open_flexbox_test "open flexbox test" ON)
if(open_flexbox_test)
    file(GLOB_RECURSE test_cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
    add_executable(flexbox_test ${test_cpp})
    target_link_libraries(flexbox_test PRIVATE gtest_main flexbox)   
endif()