﻿cmake_minimum_required(VERSION 3.0.0)

project(grid_layout VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# grid_layout
file(GLOB_RECURSE all_h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE all_hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE all_cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${all_h} ${all_hpp} ${all_cpp})
add_library(grid_layout ${all_h} ${all_hpp} ${all_cpp})
target_include_directories(grid_layout PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_target_properties(grid_layout PROPERTIES POSITION_INDEPENDENT_CODE ON)

# grid_layout_gtest
if(open_vgg_layout_test)
    file(GLOB_RECURSE test_cpp ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
    add_executable(grid_layout_gtest ${test_cpp})
    target_link_libraries(grid_layout_gtest PRIVATE gtest_main grid_layout)
endif()

install(TARGETS grid_layout COMPONENT vgg_module_grid_layout)
