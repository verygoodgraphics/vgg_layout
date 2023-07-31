cmake_minimum_required(VERSION 3.0.0)
project(vgg_layout VERSION 0.1.0)

if(NOT DEFINED CACHE{VGG_CONTRIB_YOGA_INCLUDE})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vgg_contrib/yoga)
endif()

if(NOT DEFINED CACHE{VGG_CONTRIB_BOOST_INCLUDE})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vgg_contrib/boost)
endif()

add_subdirectory(flexbox)
add_subdirectory(grid_layout)