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

#include <list>
#include <stack>
#include "wasp_gl.hpp"
#include "quad.hpp"
#include "renderable.hpp"
#include "updatable.hpp"

namespace NWasp
{
    class Effect;
    
    class Scene : public Renderable
                , public Updatable
    {
        friend class CgContext;
    private:
                        Scene               ( );
        virtual         ~Scene              ( );
                        Scene               ( const Scene&  )                   = delete;
        Scene&          operator =          ( const Scene&  )                   = delete;
        
        static Scene*   s_instance;
    public:
        //
        // Singleton methods
        //
        
        static bool     Create              ( );
        static Scene*   Instance            ( );
        static void     Destroy             ( );
        
        virtual void    Update              ( );
        virtual void    Render              ( ) const;

        void            AddRenderable       ( Renderable*  renderable );
        void            AddUpdatable        ( Updatable*   updatable  );

    private:
        //
        // Cg State
        //
        void            SetRenderTarget     ( u32 fbo );
        void            ResetRenderTarget   ( );
        bool            ValidateRenderTarget( );
        
        void            SetRenderScene      ( bool render_scene );
        void            ResetRenderScene    ( );
        bool            ValidateRenderScene ( );
        
        void            SetRenderFullscreenQuad      ( bool render_fullscreen_quad );
        void            ResetRenderFullscreenQuad    ( );
        bool            ValidateRenderFullscreenQuad ( );
        
        //
        // States
        //
        bool                    m_renderScene;
        bool                    m_renderFullscreenQuad;
        
        std::list<Updatable*>   m_updatables;
        std::list<Renderable*>  m_renderables;

        Quad*                   m_quad;

        Effect*                 m_effect;
    };
};
