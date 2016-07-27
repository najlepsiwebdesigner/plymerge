cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

ADD_DEFINITIONS(
    -std=c++11 # Or -std=c++0x
    # Other flags
)
project(plymerge)

find_package(PCL 1.2 REQUIRED)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})

FIND_PACKAGE( Boost 1.40 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_definitions(${PCL_DEFINITIONS})

add_executable (plymerge plymerge.cpp strnatcmp.hpp)
target_link_libraries (plymerge ${PCL_LIBRARIES} ${Boost_LIBRARIES})
