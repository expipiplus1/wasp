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

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )

#
# load libraries
#
find_package(                       OpenGL      REQUIRED )

set( joemath_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/joemath/include" )
set( joemath_LIBRARY      libjoemathStatic )

set( joefile_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/joefile/include" )
set( joefile_LIBRARY      libjoefileStatic )

set( joefx_INCLUDE_DIR   "${CMAKE_SOURCE_DIR}/joefx/include" )
set( joefx_LIBRARY        libjoefxStatic )

set( glfw_INCLUDE_DIR    "${CMAKE_SOURCE_DIR}/glfw/include")
set( glfw_LIBRARY         libglfwStatic )

set( glee_INCLUDE_DIR    "${CMAKE_SOURCE_DIR}/glee/include")
set( glee_LIBRARY         libgleeStatic )

#
# Create the main engine
#

set( wasp_sources       src/main.cpp
                        src/attribute_indices.hpp
                        src/camera.cpp
                        src/camera.hpp
                        src/camera_manager.cpp
                        src/camera_manager.hpp
                        #src/cg_context.cpp
                        #src/cg_context.hpp
                        src/effect.cpp
                        src/effect.hpp
                        src/effect_manager.cpp
                        src/effect_manager.hpp
                        src/fly_camera.cpp
                        src/fly_camera.hpp
                        src/input.cpp
                        src/input.hpp
                        src/jfx_context.cpp
                        src/jfx_context.hpp
                        src/mesh.cpp
                        src/mesh.hpp
                        src/model.cpp
                        src/model.hpp
                        src/quad.cpp
                        src/quad.hpp
                        src/render_target.hpp
                        src/render_target.cpp
                        src/renderable.hpp
                        src/scene.cpp 
                        src/scene.hpp
                        src/state_manager.cpp
                        src/state_manager.hpp
                        src/time.cpp
                        src/time.hpp
                        src/timer.cpp
                        src/timer.hpp
                        src/updatable.hpp
                        src/wasp_gl.hpp
                        src/window.cpp
                        src/window.hpp )

set( wasp_data_dir data )

set( wasp_effect_sources    ${wasp_data_dir}/effects/blue.cgfx
                            ${wasp_data_dir}/effects/global.cgh
                            ${wasp_data_dir}/effects/passthrough.cgfx
                            ${wasp_data_dir}/effects/phong.cgfx 
                            ${wasp_data_dir}/effects/scene.cgfx
                            ${wasp_data_dir}/effects/simplex.cgfx )


set( wasp_model_sources     ${wasp_data_dir}/models/bunny.ply )

add_executable( wasp    ${wasp_sources} ) 

if( ${CMAKE_CXX_COMPILER_ID} MATCHES "clang" )
  set( CXX11_FLAGS -std=c++0x -stdlib=libc++ )
else()
  set( CXX11_FLAGS -std=c++0x )
endif()

set_target_properties( wasp PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} ${CXX11_FLAGS}" )

include_directories( ${joemath_INCLUDE_DIR} ${joefile_INCLUDE_DIR} ${joefx_INCLUDE_DIR} ${glfw_INCLUDE_DIR} ${glee_INCLUDE_DIR} )

set( GL_LIBS ${OPENGL_LIBRARY} ${glfw_LIBRARY} ${glee_LIBRARY} )

if( UNIX AND NOT APPLE AND NOT CYGWIN )
    list( APPEND GL_LIBS rt X11 Xrandr Xxf86vm )
endif()

if( UNIX AND APPLE )
    find_library( COCOA_FRAMEWORK Cocoa )
    list( APPEND GL_LIBS ${COCOA_FRAMEWORK} )
endif()

if( WINDOWS )
    list( APPEND GL_LIBS opengl32 )
endif()

target_link_libraries( wasp ${joemath_LIBRARY} ${joefile_LIBRARY} ${joefx_LIBRARY} ${GL_LIBS} )

if( ${CMAKE_CXX_COMPILER_ID} MATCHES "clang" )
  target_link_libraries( wasp c++ )
endif()  

#
# Copy shaders
#

#foreach( FILENAME ${wasp_effect_sources} )
    #set( SRC "${CMAKE_SOURCE_DIR}/${FILENAME}" )
    #set( DST "${CMAKE_BINARY_DIR}/${FILENAME}" )
#    
    #add_custom_command( OUTPUT ${FILENAME}
                        #COMMAND ${CG_COMPILER} -noentry -strict -glslWerror -I ${CMAKE_SOURCE_DIR}/src ${SRC}
                        #COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST}
                        #DEPENDS ${SRC}
                        #IMPLICIT_DEPENDS CXX ${SRC}
                        #COMMENT "Checking ${FILENAME}" )
#endforeach( )
#
#add_custom_target( wasp_effects
                   #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/src/attribute_indices.hpp ${CMAKE_BINARY_DIR}/${wasp_data_dir}/effects/attribute_indices.hpp
                   #DEPENDS  ${wasp_effect_sources} )
#add_dependencies( wasp wasp_effects )

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

add_subdirectory( joefile EXCLUDE_FROM_ALL )

add_subdirectory( joefx EXCLUDE_FROM_ALL )

add_subdirectory( glfw EXCLUDE_FROM_ALL )

add_subdirectory( glee EXCLUDE_FROM_ALL )

#
# Create the model_compiler
#

add_subdirectory( model_compiler )
