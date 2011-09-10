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

#include "state_manager.hpp"

#include <cassert>
#include <iostream>
#include <stack>
#include <vector>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <joemath/joemath.hpp>
#include "camera.hpp"
#include "camera_manager.hpp"
#include "effect_manager.hpp"
#include "quad.hpp"
#include "renderable.hpp"
#include "window.hpp"

namespace NWasp
{
    StateManager* StateManager::s_instance = nullptr;

    StateManager::StateManager          ( )
    {
    }

    StateManager::~StateManager         ( )
    {
    }

    //
    // Singleton methods
    //

    bool     StateManager::Create          ( )
    {
        assert( s_instance == nullptr );
        s_instance = new StateManager;

        s_instance->m_renderTargets.push_back( RenderTarget( 0, 0, 0, Window::Instance()->GetSize() ) );
        s_instance->m_currentRenderTargetIndices.push( 0 );

        return true;
    }

    StateManager*  StateManager::Instance         ( )
    {
        assert( s_instance != nullptr );
        return s_instance;
    }

    void     StateManager::Destroy         ( )
    {
        assert( s_instance != nullptr );
        delete s_instance;
        s_instance = nullptr;
    }

    void    StateManager::ApplyState ( )
    {
        if( m_newRenderTarget )
        {
            u32 render_target_index = m_currentRenderTargetIndices.top();
            m_renderTargets[ render_target_index ].Bind();
            m_newRenderTarget = false;
        }
        if( m_newClearColor )
        {
            glClearColor( m_clearColor.x(), m_clearColor.y(), m_clearColor.z(), m_clearColor.w() );
            std::cout << "new clearcolor " << m_clearColor.x() << "\n";
            m_newClearColor = false;
        }
        if( m_clear )
        {
            std::cout << "clearing\n";
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            m_clear = false;
        }
    }

    u32     StateManager::AddRenderTarget( RenderTarget render_target )
    {
        m_renderTargets.push_back( render_target );
        return m_renderTargets.size() - 1;
    }

    bool    StateManager::GetRenderScene() const
    {
        return m_renderScene;
    }

    bool StateManager::GetRenderFullscreenQuad() const
    {
        return m_renderFullscreenQuad;
    }

    //
    // Cg State
    //
    
    void            StateManager::SetClearColor      ( const float4& clear_color )
    {
        m_clearColor = clear_color;
        m_newClearColor = true;
    }
    
    void            StateManager::ResetClearColor    ( )
    {
        m_clearColor = float4( 0.0f, 0.0f, 0.0f, 0.0f );
        m_newClearColor = true;
    }
    
    bool            StateManager::ValidateClearColor ( ) const
    {
        return true;
    }
   
    
    void            StateManager::SetClear      ( const bool clear )
    {
        m_clear = clear;
    }
    
    void            StateManager::ResetClear    ( )
    {
        m_clear = false;
    }
    
    bool            StateManager::ValidateClear ( ) const
    {
        return true;
    }

    
    void            StateManager::SetRenderTarget      ( const u32 render_target )
    {
        m_currentRenderTargetIndices.push( render_target );
        m_newRenderTarget = true;
    }
    
    void            StateManager::ResetRenderTarget    ( )
    {
        m_currentRenderTargetIndices.pop();    
        m_newRenderTarget = true;
    }
    
    bool            StateManager::ValidateRenderTarget ( ) const
    {
        return true;
    }


    void            StateManager::SetRenderScene      ( const bool render_scene )
    {
        m_renderScene = render_scene;
    }
    
    void            StateManager::ResetRenderScene    ( )
    {
        m_renderScene = false;
    }
    
    bool            StateManager::ValidateRenderScene ( ) const
    {
        return true;
    }


    void            StateManager::SetRenderFullscreenQuad      ( const bool render_fullscreen_quad )
    {
        m_renderFullscreenQuad = render_fullscreen_quad;
    }
    
    void            StateManager::ResetRenderFullscreenQuad    ( )
    {
        m_renderFullscreenQuad = false;
    }
    
    bool            StateManager::ValidateRenderFullscreenQuad ( ) const
    {
        return true;
    }
};
