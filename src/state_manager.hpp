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

#pragma once

#include <stack>
#include <vector>
#include <joemath/joemath.hpp>
#include "render_target.hpp"
#include "wasp_gl.hpp"

namespace NWasp
{
    class StateManager
    {
    private:
                        StateManager               ( );
        virtual         ~StateManager              ( );
                        StateManager               ( const StateManager&  )                   = delete;
        StateManager&   operator =                 ( const StateManager&  )                   = delete;
        
        static StateManager*   s_instance;
    public:
        //
        // Singleton methods
        //
        
        static bool     Create              ( );
        static StateManager*   Instance            ( );
        static void     Destroy             ( );

        void ApplyState();

        u32 AddRenderTarget( RenderTarget render_target );

        bool GetRenderScene() const;
        bool GetRenderFullscreenQuad() const;
        
    private:
        //
        // Cg State
        //
        friend class CgContext;

        void            SetClearColor     ( const float4& clear_color );
        void            ResetClearColor   ( );
        bool            ValidateClearColor( ) const;

        void            SetClear     ( bool clear );
        void            ResetClear   ( );
        bool            ValidateClear( ) const;

        void            SetRenderTarget     ( const u32 render_target );
        void            ResetRenderTarget   ( );
        bool            ValidateRenderTarget( ) const;
        
        void            SetRenderScene      ( const bool render_scene );
        void            ResetRenderScene    ( );
        bool            ValidateRenderScene ( ) const;
        
        void            SetRenderFullscreenQuad      ( const bool render_fullscreen_quad );
        void            ResetRenderFullscreenQuad    ( );
        bool            ValidateRenderFullscreenQuad ( ) const;
        
        //
        // States
        //
        bool                    m_renderScene = false;
        bool                    m_renderFullscreenQuad = false;
        
        std::vector<RenderTarget>   m_renderTargets;
        std::stack<u32>             m_currentRenderTargetIndices;
        bool                        m_newRenderTarget = true;

        float4                      m_clearColor = float4( 0.0f, 0.0f, 0.0f, 0.0f );
        bool                        m_newClearColor = true;

        bool                        m_clear = false;
    };
};
