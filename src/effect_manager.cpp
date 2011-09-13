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

#include "effect_manager.hpp"

#include <cassert>
#include <string>
#include "effect.hpp"

namespace NWasp
{
    EffectManager* EffectManager::s_instance = nullptr;

    EffectManager::EffectManager( )
    {
    }

    EffectManager::~EffectManager( )
    {
    }

    bool        EffectManager::Create     ( )
    {
        assert( s_instance == nullptr );
        s_instance = new EffectManager();

        s_instance->m_sceneEffect = nullptr;
        
        return true;
    }

    EffectManager*    EffectManager::Instance   ( )
    {
        assert( s_instance != nullptr );
        return s_instance;
    }

    void        EffectManager::Destroy    ( )
    {
        assert( s_instance != nullptr );
        delete s_instance;
        s_instance = nullptr;
    }

    Effect*        EffectManager::LoadEffect( std::string filename, bool is_scene_effect )
    {
        Effect* effect;
        if( m_effects.count( filename ) != 0 )
        {
            //
            // We have already loaded this effect, return that
            //
            effect = &m_effects[filename];
        }
        else
        {
            Effect& new_effect = m_effects[filename];
            new_effect = Effect();
            new_effect.Load( filename );
            
            effect = &new_effect;
        }
        
        if( is_scene_effect )
        {
            //
            // If this is a scene level effect link all current and future parameters to matching parameters in this file
            //
            m_sceneEffect = effect;
            
            for( auto i : m_effects )
            {
                if( i.second.GetCgEffect() != m_sceneEffect->GetCgEffect() )
                {
                    CGeffect scene_effect = effect->GetCgEffect();
                    CGeffect material_effect = i.second.GetCgEffect();
                    ConnectParameters( scene_effect, material_effect );
                }
            }
        }
        else
        {
            //
            // If this isn't a scene effect and we have a scene effect then link the parameters
            //
            if( m_sceneEffect != nullptr )
            {
                CGeffect scene_effect = m_sceneEffect->GetCgEffect();
                CGeffect material_effect = effect->GetCgEffect();
                ConnectParameters( scene_effect, material_effect );
            }
        }

        if( is_scene_effect )
        {
            effect->InitializeConstants();
        }

        //
        // Create required textures and render targets
        //

        effect->AllocateBuffers();
        
        return effect;
    }
    
    void EffectManager::ConnectParameters(CGeffect scene, CGeffect material) const
    {
        CGparameter param = cgGetFirstEffectParameter( scene );
        
        while( param != NULL )
        {
            CGtype      type = cgGetParameterNamedType( param );
            std::string name = cgGetParameterName( param );
            std::string semantic = cgGetParameterSemantic( param );
            
            CGparameter sub_param = cgGetFirstEffectParameter( material );
            
            while( sub_param != NULL )
            {
                if( name == cgGetParameterName( sub_param ) &&
                    semantic == cgGetParameterSemantic( sub_param ) &&
                    type == cgGetParameterType( sub_param ) )
                {
                    if( cgGetConnectedParameter( sub_param ) == NULL )
                        cgDisconnectParameter( sub_param );
                    
                    cgConnectParameter( param, sub_param );
                    break;
                }
                
                sub_param = cgGetNextParameter( sub_param );
            }
            
            param = cgGetNextParameter( param );
        }
    }
};
