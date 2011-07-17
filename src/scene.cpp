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

#include "scene.hpp"

#include <cassert>
#include <iostream>
#include <list>
#include <Cg/cg.h>
#include <joemath/joemath.hpp>
#include "camera.hpp"
#include "camera_manager.hpp"
#include "effect_manager.hpp"
#include "renderable.hpp"

namespace NWasp
{
    Scene* Scene::s_instance = nullptr;

    Scene::Scene          ( )
    {
    }

    Scene::~Scene         ( )
    {
    }

    //
    // Singleton methods
    //

    bool     Scene::Create          ( )
    {
        assert( s_instance == nullptr );
        s_instance = new Scene;
        
        s_instance->m_effect = EffectManager::Instance()->LoadEffect( "effects/scene.cgfx", true);
        
        return true;
    }

    Scene*  Scene::Instance         ( )
    {
        assert( s_instance != nullptr );
        return s_instance;
    }

    void     Scene::Destroy         ( )
    {
        assert( s_instance != nullptr );
        delete s_instance;
        s_instance = nullptr;
    }

    void     Scene::AddRenderable( Renderable* renderable )
    {
        m_renderables.push_back( renderable );
    }
    
    void     Scene::AddUpdatable ( Updatable* updatable )
    {
        m_updatables.push_back( updatable );
    }
    
    void     Scene::Update          ( )
    {
        for( auto i : m_updatables )
            i->Update();
    }

    void     Scene::Render          ( ) const
    {
        m_effect->SetParameterBySemantic( CameraManager::Instance()->GetCurrentCamera()->GetPosition(),    "CAMERAPOSITION" );
        
        CGeffect    effect      = m_effect->GetCgEffect();
        CGtechnique technique   = cgGetFirstTechnique( effect );
        CGpass      pass        = cgGetFirstPass( technique );
        
        while( pass != NULL )
        {
            cgSetPassState( pass );
            
            if( m_renderScene )
            {
                for( auto r : m_renderables )
                    r->Render();
            }
            cgResetPassState( pass );
            
            pass = cgGetNextPass( pass );
        }
    }
    
    //
    // Cg State
    //
    
    void            Scene::SetRenderScene      ( bool render_scene )
    {
        m_renderScene = render_scene;
    }
    
    void            Scene::ResetRenderScene    ( )
    {
        //TODO should this be popping the top off the stack?
        m_renderScene = false;
    }
    
    bool            Scene::ValidateRenderScene ( )
    {
        return true;
    }
};
