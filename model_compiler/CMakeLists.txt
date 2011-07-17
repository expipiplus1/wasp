cmake_minimum_required(VERSION 2.8)

#
# SOURCE_GROUP
#
project( model_compiler )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -g" )

set( model_compiler_sources       src/main.cpp )

add_executable( model_compiler    ${model_compiler_sources} ) 

include_directories( ${joemath_INCLUDE_DIR} )

target_link_libraries( model_compiler ${joemath_LIBRARY} )

