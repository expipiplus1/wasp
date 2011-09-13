cmake_minimum_required(VERSION 2.8)

#
# SOURCE_GROUP
#
project( model_compiler )

add_subdirectory( assimp EXCLUDE_FROM_ALL )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )

set( model_compiler_sources src/main.cpp 
                            src/compiler.hpp
                            src/compiler.cpp
                            src/output_stream.hpp   
                            src/output_stream.cpp   
                            src/output_stream-inl.hpp )

add_executable( model_compiler    ${model_compiler_sources} ) 
target_link_libraries( model_compiler assimp )

set_target_properties( model_compiler PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++" )

include_directories( ${joemath_INCLUDE_DIR} )
include_directories( assimp/include )

target_link_libraries( model_compiler ${joemath_LIBRARY} c++ )

