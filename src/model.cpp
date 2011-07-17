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

#include "model.hpp"

#include <joemath/joemath.hpp>
#include "camera.hpp"
#include "camera_manager.hpp"
#include "effect_manager.hpp"
#include "mesh.hpp"
#include "time.hpp"

using namespace NJoeMath;

namespace NWasp
{
    Model::Model            ( )
    {
        m_renderSets.resize( m_renderSets.size() + 1 );
        
        m_renderSets[ m_renderSets.size() - 1 ].first = EffectManager::Instance()->LoadEffect( "effects/phong.cgfx" );
        m_renderSets[ m_renderSets.size() - 1 ].second = new Mesh;
    }
    
    Model::~Model           ( )
    {
        for( auto r : m_renderSets )
            delete r.second;
    }
    
    void    Model::Render   ( ) const
    {   
        for( auto r : m_renderSets )
        {
            float4x4 modelMatrix = RotateZXY<float,4>( 0.0f, NTime::GetApplicationTime() / 2.0, 0.0f )
                       * (Scale( float4(-10.0f, -10.0f, 10.0f, 1.0f) ) 
                       * Translate( float3(0.0f, 1.25f, 0.3f) ) );
                        
            float4x4 modelViewProjection = modelMatrix * CameraManager::Instance()->GetCurrentCamera()->GetViewProjection();
        
            r.first->SetModelViewProjection( modelViewProjection );
            r.first->SetModel              ( modelMatrix );
            
            r.first->SetParameterBySemantic( float3(0.1f, 0.1f, 0.1f),          "AMBIENTCOLOR" );
            r.first->SetParameterBySemantic( float3(0.5f, 0.5f, 1.0f),          "DIFFUSECOLOR" );
            r.first->SetParameterBySemantic( float3(1.0f, 0.0f, -2.0f),         "LIGHTPOSITION" );
            r.first->SetParameterBySemantic( 52.0f,                             "SPECULAREXPONENT" );

            r.first->RenderPrimitive( r.second );
        }
    }
};
