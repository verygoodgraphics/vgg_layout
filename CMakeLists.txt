cmake_minimum_required(VERSION 3.13.0)
project(vgg_layout VERSION 0.1.0)

if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

option(open_vgg_layout_test "open vgg_layout test" ON)
if(open_vgg_layout_test)
    include(FetchContent)
    include(GoogleTest)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
    )
    FetchContent_MakeAvailable(googletest)    
endif()

if(NOT DEFINED CACHE{VGG_CONTRIB_YOGA_INCLUDE})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vgg_contrib/yoga)
endif()

add_subdirectory(flexbox)
add_subdirectory(grid_layout)