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
# TODO: Write Find*.cmake files for glfw and joemath
#
find_package(                       OpenGL      REQUIRED )

find_package(                       Cg          REQUIRED )

add_subdirectory( joemath EXCLUDE_FROM_ALL )

add_subdirectory( glfw EXCLUDE_FROM_ALL )

set( joemath_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/joemath/include" )
set( joemath_LIBRARY      libjoemathStatic )

set( glfw_INCLUDE_DIR    "${CMAKE_SOURCE_DIR}/glfw/include")
set( glfw_LIBRARY         libglfwStatic )

#
# Create the main engine
#

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -g" )

set( wasp_sources       src/main.cpp
                        src/camera.hpp
                        src/camera.cpp
                        src/cg_context.hpp
                        src/cg_context.cpp
                        src/input.hpp
                        src/input.cpp
                        src/mesh.hpp
                        src/mesh.cpp
                        src/renderable.hpp
                        src/scene.hpp
                        src/scene.cpp
                        src/shader.hpp
                        src/shader.cpp
                        src/time.hpp
                        src/time.cpp
                        src/timer.hpp
                        src/timer.cpp
                        src/updatable.hpp
                        src/window.hpp
                        src/window.cpp )

set( wasp_effect_sources    effects/global.cgh
                            effects/phong.cgfx 
                            effects/blue.cgfx )

add_executable( wasp    ${wasp_sources} ) 

add_custom_target( wasp_shaders
                   SOURCES  ${wasp_effect_sources} )

add_dependencies( wasp wasp_shaders )

include_directories( ${joemath_INCLUDE_DIR} ${glfw_INCLUDE_DIR} ${CG_INCLUDE_DIR} )

set( GL_LIBS ${OPENGL_LIBRARY} ${glfw_LIBRARY} ${CG_LIBRARY} ${CG_GL_LIBRARY} )

if( UNIX )
    list( APPEND GL_LIBS Xrandr Xxf86vm )
else()
    list( APPEND GL_LIBS opengl32 )
endif()

target_link_libraries( wasp ${joemath_LIBRARY} ${GL_LIBS} )

#
# Copy shaders
#

foreach( FILENAME ${wasp_effect_sources} )
    set( SRC "${CMAKE_CURRENT_SOURCE_DIR}/${FILENAME}" )
    set( DST "${CMAKE_BINARY_DIR}/${FILENAME}" )

    add_custom_command( TARGET wasp_shaders
                        COMMAND ${CG_COMPILER} -noentry -strict -glslWerror ${SRC}
                        COMMAND ${CMAKE_COMMAND} -E copy ${SRC} ${DST}
                        COMMENT "Checking ${SRC}" )
endforeach( )

