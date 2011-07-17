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

#include "effect.hpp"

#include <iostream>
#include <string>
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <GL/glfw3.h>
#include <joemath/joemath.hpp>
#include "cg_context.hpp"
#include "renderable.hpp"

using namespace NJoeMath;

namespace NWasp
{
    Effect::Effect    ( )
    {
    }

    Effect::~Effect   ( )
    {
    }

    bool            Effect::Load    ( std::string filename )
    {        
        m_filename = filename;
        m_cgEffect = cgCreateEffectFromFile( CgContext::Instance( )->GetCgContext( ), filename.c_str(), nullptr );

        CGtechnique technique = cgGetFirstTechnique( m_cgEffect );
        CGbool valid;
        valid = cgValidateTechnique( technique );
        
        if (!valid)
        {
            std::cerr << "Couldn't validate technique" << std::endl;
            return false;
        }
               
        return true;
    }
    
    bool            Effect::Reload  ( )
    {
        CGeffect old_effect = m_cgEffect;
        if( !Load( m_filename ) )
        {
            m_cgEffect = old_effect;
            return false;
        }
        return true;
    }

    std::string     Effect::GetName ( ) const
    {
        return std::string( cgGetEffectName( m_cgEffect ) );
    }        

    void            Effect::Bind    ( ) const
    {
        CGtechnique technique = cgGetFirstTechnique( m_cgEffect );
        CGpass      pass = cgGetFirstPass( technique );
        cgSetPassState( pass );
    }
    
    CGeffect        Effect::GetCgEffect()
    {
        return m_cgEffect;
    }
    
    void            Effect::RenderPrimitive( Renderable* primitive ) const
    {
        CGtechnique technique = cgGetFirstTechnique( m_cgEffect );
        CGpass      pass      = cgGetFirstPass( technique );
        
        while( pass != NULL )
        {
            cgSetPassState( pass );
            
            primitive->Render();
            
            cgResetPassState( pass );
            pass = cgGetNextPass( pass );
        }
    }

    void            Effect::SetModelViewProjection ( const float4x4& modelViewProjection ) const
    {
        CGparameter param = cgGetEffectParameterBySemantic( m_cgEffect, "MODELVIEWPROJECTION" );
        cgSetMatrixParameterfr( param, reinterpret_cast<const float*>(&modelViewProjection) );
    }

    void            Effect::SetModel               ( const float4x4& model ) const
    {
        CGparameter param = cgGetEffectParameterBySemantic( m_cgEffect, "MODEL" );
        cgSetMatrixParameterfr( param, reinterpret_cast<const float*>(&model) );
    }

    void            Effect::SetParameterBySemantic    ( const float3& v, const char* semantic ) const
    {
        CGparameter param = cgGetEffectParameterBySemantic( m_cgEffect, semantic );
        cgSetParameter3fv( param, reinterpret_cast<const float*>(&v) );
    }

    void            Effect::SetParameterBySemantic    ( const float v, const char* semantic ) const
    {
        CGparameter param = cgGetEffectParameterBySemantic( m_cgEffect, semantic );
        cgSetParameter1f( param, v );
    }
};
