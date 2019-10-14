project( clo )

cmake_minimum_required(VERSION 3.12)

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )

add_subdirectory( inc )
add_subdirectory( libs )

add_subdirectory( test )
add_subdirectory( examples )