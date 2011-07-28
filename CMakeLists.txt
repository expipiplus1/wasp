cmake_minimum_required(VERSION 2.8)

#
# CMake Policies
#
cmake_policy(VERSION 2.8)

#
# SOURCE_GROUP
#
project( wasp )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")
include(${CMAKE_MODULE_PATH}/DetermineProcessor.cmake)

#
# load libraries
#
find_package(                       OpenGL      REQUIRED )

find_package(                       Cg          REQUIRED )

set( joemath_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/joemath/include" )
set( joemath_LIBRARY      libjoemathStatic )

set( glfw_INCLUDE_DIR    "${CMAKE_SOURCE_DIR}/glfw/include")
set( glfw_LIBRARY         libglfwStatic )

set( glee_INCLUDE_DIR    "${CMAKE_SOURCE_DIR}/glee/include")
set( glee_LIBRARY         libgleeStatic )

#
# Create the model_compiler
#
add_subdirectory( model_compiler )

#
# Create the main engine
#

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -g" )

set( wasp_sources       src/main.cpp
                        src/attribute_indices.hpp
                        src/camera.hpp
                        src/camera.cpp
                        src/camera_manager.hpp
                        src/camera_manager.cpp
                        src/cg_context.hpp
                        src/cg_context.cpp
                        src/effect.hpp
                        src/effect.cpp
                        src/effect_manager.hpp
                        src/effect_manager.cpp
                        src/fly_camera.hpp
                        src/fly_camera.cpp
                        src/input.hpp
                        src/input.cpp
                        src/input_stream.hpp
                        src/input_stream.cpp
                        src/input_stream-inl.hpp
                        src/mesh.hpp
                        src/mesh.cpp
                        src/model.hpp
                        src/model.cpp
                        src/renderable.hpp
                        src/scene.hpp
                        src/scene.cpp 
                        src/time.hpp
                        src/time.cpp
                        src/timer.hpp
                        src/timer.cpp
                        src/updatable.hpp
                        src/window.hpp
                        src/window.cpp )

set( wasp_data_dir data )

set( wasp_effect_sources    ${wasp_data_dir}/effects/global.cgh
                            ${wasp_data_dir}/effects/scene.cgfx
                            ${wasp_data_dir}/effects/phong.cgfx 
                            ${wasp_data_dir}/effects/blue.cgfx )

set( wasp_model_sources     ${wasp_data_dir}/models/bunny.ply )

add_executable( wasp    ${wasp_sources} ) 

include_directories( ${joemath_INCLUDE_DIR} ${glfw_INCLUDE_DIR} ${glee_INCLUDE_DIR} ${CG_INCLUDE_DIR} )

set( GL_LIBS ${OPENGL_LIBRARY} ${glfw_LIBRARY} ${glee_LIBRARY} ${CG_LIBRARY} ${CG_GL_LIBRARY} )

if( UNIX )
    list( APPEND GL_LIBS X11 Xrandr Xxf86vm )
else()
    list( APPEND GL_LIBS opengl32 )
endif()

target_link_libraries( wasp ${joemath_LIBRARY} ${GL_LIBS} )

#
# Copy shaders
#

foreach( FILENAME ${wasp_effect_sources} )
    set( SRC "${CMAKE_SOURCE_DIR}/${FILENAME}" )
    set( DST "${CMAKE_BINARY_DIR}/${FILENAME}" )
    
    add_custom_command( OUTPUT ${FILENAME}
                        COMMAND ${CG_COMPILER} -noentry -strict -glslWerror -I ${CMAKE_SOURCE_DIR}/src ${SRC}
                        COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST}
                        DEPENDS ${SRC}
                        IMPLICIT_DEPENDS CXX ${SRC}
                        COMMENT "Checking ${FILENAME}" )
endforeach( )

add_custom_target( wasp_effects
                   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/attribute_indices.hpp ${CMAKE_BINARY_DIR}/${wasp_data_dir}/effects/attribute_indices.hpp
                   DEPENDS  ${wasp_effect_sources} )
add_dependencies( wasp wasp_effects )

#
# Compile models
#

foreach( FILENAME ${wasp_model_sources} )
    set( SRC "${CMAKE_SOURCE_DIR}/${FILENAME}" )
    get_filename_component( filename_without_extension ${FILENAME} NAME_WE )
    get_filename_component( file_path ${FILENAME} PATH )
    set( FILENAME "${file_path}/${filename_without_extension}.joe" )
    file( MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/${file_path}/" )
    set( DST "${CMAKE_BINARY_DIR}/${FILENAME}" )
    
    list( APPEND wasp_models_compiled ${FILENAME} )

    add_custom_command( OUTPUT ${FILENAME}
                        COMMAND model_compiler ${SRC} ${DST}
                        COMMENT "Compiling ${FILENAME}" )
endforeach( )

MESSAGE( STATUS ${wasp_models_compiled} )

add_custom_target( wasp_models
                   DEPENDS ${wasp_models_compiled} )
add_dependencies( wasp wasp_models )
#
# Uninstall operation
#
if( NOT TARGET uninstall )
    configure_file( ${wasp_SOURCE_DIR}/cmake_uninstall.cmake.in
                    ${wasp_BINARY_DIR}/cmake_uninstall.cmake IMMEDIATE @ONLY )

    add_custom_target( uninstall
                       ${CMAKE_COMMAND} -P
                       ${wasp_BINARY_DIR}/cmake_uninstall.cmake )
endif( )
  
#
# submodules
#

add_subdirectory( joemath EXCLUDE_FROM_ALL )

add_subdirectory( glfw EXCLUDE_FROM_ALL )

add_subdirectory( glee EXCLUDE_FROM_ALL )
