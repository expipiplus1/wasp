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

#include <iostream>
#include <GL/glfw3.h>
#include <joemath/joemath.hpp>

namespace NWasp
{
    const u32   INIT_WIDTH  = 640;
    const u32   INIT_HEIGHT = 640;
    const char* INIT_WINDOW_TITLE = "wasp";

    CWindow::CWindow( )
    {
        m_initialized = false;
    }

    CWindow::~CWindow( )
    {
        if ( m_initialized )
            glfwTerminate( );
    }

    bool CWindow::Init( )
    {
        s32 error;
        
        //
        // Initialise GLFW
        //
        
        glfwInit( );
        error = glfwGetError( );
        if ( error != GLFW_NO_ERROR )
        {
            std::cerr << "GLFW initialization failed: " << glfwErrorString( error ) << std::endl;
            glfwTerminate( );
            return false;
        }
        
        m_window = glfwOpenWindow( INIT_WIDTH, INIT_HEIGHT, GLFW_WINDOWED, INIT_WINDOW_TITLE, NULL );
        if ( !m_window )
        {
            std::cerr << "GLFW window creation failed: " << glfwErrorString( error ) << std::endl;
            glfwTerminate( );
            return false;
        }
        
        glfwGetWindowSize( m_window, &m_width, &m_height );

        glfwEnable( m_window, GLFW_STICKY_KEYS );
        
        glfwSwapInterval( 1 );
        
        glClearColor ( 1.0f, 0.93f, 0.44f, 1.0f );
        
        m_initialized = true;
        
        return true;
    }

    bool CWindow::IsWindowClosed() const
    {
        if ( !m_initialized )
            return true;
        
        return ( !glfwIsWindow( m_window ) ||
                glfwGetKey( m_window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ||
                glfwGetKey( m_window, 'Q' )             == GLFW_PRESS );
    }

    void CWindow::Swap()
    {
        if ( !m_initialized )
            return;

        //
        // Swap buffers
        //
        glfwSwapBuffers( );
        
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        //
        // Poll for keypresses
        //
        glfwPollEvents( );
    }

    void CWindow::SetTitle( const char* title ) const
    {
        glfwSetWindowTitle( m_window, title );
    }
};
