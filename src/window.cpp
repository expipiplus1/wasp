/*
    Copyright 2011 Joe Hermaszewski. All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, are
    permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this list of
    conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice, this list
    of conditions and the following disclaimer in the documentation and/or other materials
    provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY Joe Hermaszewski "AS IS" AND ANY EXPRESS OR IMPLIED
    WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
    FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Joe Hermaszewski OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    The views and conclusions contained in the software and documentation are those of the
    authors and should not be interpreted as representing official policies, either expressed
    or implied, of Joe Hermaszewski.
*/

#include "window.hpp"

#include <cassert>
#include <iostream>
#include "wasp_gl.hpp"
#include <joemath/joemath.hpp>

using namespace JoeMath;

namespace NWasp
{
    Window* Window::s_instance = nullptr;

    const u32   INIT_WIDTH  = 640;
    const u32   INIT_HEIGHT = 640;
    const char* INIT_WINDOW_TITLE = "wasp";

    Window::Window( )
    {
    }

    Window::~Window( )
    {
    }

    bool        Window::Create     ( )
    {
        assert( s_instance == nullptr );
        s_instance = new Window();

        s32 error;
        
        //
        // Initialise GLFW
        //
        
        glfwInit( );
        error = glfwGetError( );
        if ( error != GLFW_NO_ERROR )
        {
            std::cerr << "GLFW initialization failed: " << glfwErrorString( error ) << std::endl;
            Destroy(); 
            return false;
        }

         glfwOpenWindowHint( GLFW_DEPTH_BITS, 32 );

         glfwOpenWindowHint( GLFW_OPENGL_VERSION_MAJOR, 3 );
         glfwOpenWindowHint( GLFW_OPENGL_VERSION_MINOR, 2 );
         glfwOpenWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        
        s_instance->m_window = glfwOpenWindow( INIT_WIDTH, INIT_HEIGHT, GLFW_WINDOWED, INIT_WINDOW_TITLE, nullptr );
        if ( !s_instance->m_window )
        {
            std::cerr << "GLFW window creation failed: " << glfwErrorString( error ) << std::endl;
            Destroy(); 
            return false;
        }
        
        glfwGetWindowSize( s_instance->m_window, &s_instance->m_width, &s_instance->m_height );

        glfwEnable( s_instance->m_window, GLFW_STICKY_KEYS );
        glEnable( GL_TEXTURE_2D );
        glDisable( GL_DEPTH_TEST );
        
        glfwSwapInterval( 1 );

        return true;
    }

    Window*    Window::Instance   ( )
    {
        assert( s_instance != nullptr );
        return s_instance;
    }

    void        Window::Destroy    ( )
    {
        assert( s_instance != nullptr );
        glfwTerminate( );

        delete s_instance;
        s_instance = nullptr;
    }

    bool Window::IsWindowClosed() const
    {
        return ( !glfwIsWindow( m_window ) ||
                glfwGetKey( m_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ||
                glfwGetKey( m_window, 'Q' )             == GLFW_PRESS );
    }

    void Window::Swap()
    {
        //
        // Swap buffers
        //
        glfwSwapBuffers( );
        
        //
        // Poll for keypresses
        //
        glfwPollEvents( );
    }

    int2 Window::GetSize ( ) const
    {
        return int2( m_width, m_height );
    }

    void Window::SetTitle( const char* title ) const
    {
        glfwSetWindowTitle( m_window, title );
    }
    
    const GLFWwindow  Window::GetWindow() const
    {
        return m_window;
    }
};
