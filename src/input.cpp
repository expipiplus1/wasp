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

#include "input.hpp"

#include <cassert>
#include "wasp_gl.hpp"
#include "window.hpp"

namespace NWasp
{
    Input* Input::s_instance = nullptr;
    
    Input::Input                  ( )
    {
    }

    Input::~Input                 ( )
    {
    }

    bool            Input::Create          ( )
    {
        assert( s_instance == nullptr );
        s_instance = new Input();
        
        //
        // Get windows size
        //
        int2 integer_window_size;
        glfwGetWindowSize( const_cast<GLFWwindow>(Window::Instance()->GetWindow()), &integer_window_size[0], &integer_window_size[1] );
        s_instance->m_windowSize = integer_window_size;
        
        int2 integer_mouse_position;
        glfwGetMousePos( Window::Instance()->GetWindow(), &integer_mouse_position[0], &integer_mouse_position[1] );
        s_instance->m_mousePosition = integer_mouse_position / s_instance->m_windowSize;
        s_instance->m_mousePreviousPosition = s_instance->m_mousePosition;
        s_instance->m_deltaTime = 1.0f;
        
        return true;
    }
    
    Input*     Input::Instance        ( )
    {
        assert( s_instance != nullptr );
        return s_instance;
    }

    void            Input::Destroy         ( )
    {
        assert( s_instance != nullptr );
        delete s_instance;
        s_instance = nullptr;
    }
    
    void    Input::Poll                ( )
    {
        glfwPollEvents();
        
        GLFWwindow window = const_cast<GLFWwindow>(Window::Instance()->GetWindow());
        
        for( u32 i = 0; i < GLFW_KEY_LAST; ++i )
        {
            bool last_state = m_keyStates[i];
            m_keyStates[i]  = glfwGetKey( window, i ) == GLFW_PRESS;
            m_keyChanged[i] = m_keyStates[i] != last_state;
        }
              
        int2 integer_mouse_position;
        glfwGetMousePos( window, &integer_mouse_position[0], &integer_mouse_position[1] );
        m_mousePreviousPosition = m_mousePosition;
        m_mousePosition = integer_mouse_position / m_windowSize;
        
        float current_time = glfwGetTime();
        m_deltaTime = current_time - m_previousTime;
        m_previousTime = current_time;
    }

    float2  Input::GetMousePosition    ( ) const
    {
        return m_mousePosition;
    }

    float2  Input::GetMouseVelocity    ( ) const
    {
        return (m_mousePosition - m_mousePreviousPosition) / m_deltaTime;
    }
    
    float2  Input::GetMouseDelta       ( ) const
    {
        return m_mousePosition - m_mousePreviousPosition;
    }

    bool    Input::IsKeyDown           ( const int key ) const
    {
        return m_keyStates[key];
    }
    
    bool    Input::IsKeyChanged        ( const int key ) const
    {
        return m_keyChanged[key];
    }
};
