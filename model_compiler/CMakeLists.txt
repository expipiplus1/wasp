cmake_minimum_required(VERSION 2.8)

#
# SOURCE_GROUP
#
project( model_compiler )

add_subdirectory( assimp EXCLUDE_FROM_ALL )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )

set( model_compiler_sources src/main.cpp 
                            src/compiler.hpp
                            src/compiler.cpp )

add_executable( model_compiler    ${model_compiler_sources} ) 
target_link_libraries( model_compiler assimp ${joefile_LIBRARY} ${joemath_LIBRARY} )

if( ${CMAKE_CXX_COMPILER_ID} MATCHES "clang" )
  target_link_libraries( model_compiler c++ )
endif()

set_target_properties( model_compiler PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}" )

include_directories( ${joemath_INCLUDE_DIR} )
include_directories( ${joefile_INCLUDE_DIR} )
include_directories( assimp/include )

