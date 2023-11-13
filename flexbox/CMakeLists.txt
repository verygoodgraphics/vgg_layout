cmake_minimum_required(VERSION 3.0.0)

project(flexbox VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# flexbox
file(GLOB_RECURSE all_h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE all_hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE all_cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${all_h} ${all_hpp} ${all_cpp})
add_library(flexbox ${all_h} ${all_hpp} ${all_cpp})
target_link_libraries(flexbox PUBLIC yogacore)
target_include_directories(flexbox PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# flexbox_test
if(open_vgg_layout_test)
    file(GLOB_RECURSE test_cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
    add_executable(flexbox_test ${test_cpp})
    target_link_libraries(flexbox_test PRIVATE gtest_main flexbox)   
endif()

install(TARGETS flexbox yogacore COMPONENT vgg_flexbox)