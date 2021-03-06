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

#include <string>
#include <vector>
#include <joemath/joemath.hpp>
#include <joefx/effect.hpp>
#include "render_target.hpp"

using namespace JoeMath;

namespace NWasp
{
    class Renderable;
    
    class Effect
    {
    public:
                        Effect                  ( );
        virtual         ~Effect                 ( );
        
        bool            Load                    ( std::string     filename );
        bool            Reload                  ( );

        JoeFx::Effect*  GetJfxEffect            ( );
        std::string     GetName                 ( ) const;
        
        void            RenderPrimitive         ( Renderable* primitive ) const;
        
        void            Bind                    ( ) const;

        void            SetModelViewProjection  ( const float4x4& modelViewProjection ) const;
        void            SetModel                ( const float4x4& model ) const;
        void            SetParameterBySemantic  ( const float3&   v,
                                                  const char*     semantic ) const;
        void            SetParameterBySemantic  ( const float     v,
                                                  const char*     semantic ) const;
        
        void            InitializeConstants     ( ) const;
        void            AllocateBuffers         ( );
    private:
        std::string                 m_name;
        std::string                 m_filename;
        JoeFx::Effect*              m_jfxEffect;

        std::vector<RenderTarget>   m_renderTargets;
    };
};
