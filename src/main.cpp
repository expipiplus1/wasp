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

#include <cstdio>
#include <iostream>
#include <joemath/joemath.hpp>
#include <joefx/context.hpp>
#include "camera_manager.hpp"
#include "effect_manager.hpp"
#include "fly_camera.hpp"
#include "input.hpp"
#include "jfx_context.hpp"
#include "model.hpp"
#include "scene.hpp"
#include "state_manager.hpp"
#include "time.hpp"
#include "window.hpp"

using namespace JoeMath;

int main (int argc, char** argv)
{
    if ( !NWasp::Window::Create() )
        return 1;

    if ( !NWasp::JfxContext::Create() )
        return 2;

    if ( !NWasp::StateManager::Create() )
        return 7;
    
    if ( !NWasp::EffectManager::Create() )
        return 3;
    
    if ( !NWasp::Input::Create() )
        return 4;

    if ( !NWasp::Scene::Create() )
        return 5;

    if ( !NWasp::CameraManager::Create() )
        return 6;

    NWasp::FlyCamera camera;

    camera.SetPosition( float3(1.0f, 0.0f, -2.0f) );
    camera.SetTarget( float3(0.0f, 0.35f, 0.0f) );
    camera.SetAspectRatio( 1.0f );
    camera.SetVerticalFov( DegToRad(70.0f) );
    camera.SetNearPlane( 0.01f );
    camera.SetFarPlane( 100.0f );
    camera.RecalculateMatrixStack();

    NWasp::CameraManager::Instance()->SetCurrentCamera( &camera );
    NWasp::Scene::Instance()->AddUpdatable( &camera );
    
    NWasp::Model model;
    model.Load( "data/models/bunny.joe" );
    NWasp::Scene::Instance()->AddRenderable( &model );
    
    double old_time = NTime::GetApplicationTime();

    while ( !NWasp::Window::Instance()->IsWindowClosed( ) )
    {      
        NWasp::Scene::Instance()->Update();
        NWasp::Scene::Instance()->Render();     

        NWasp::Window::Instance()->Swap( );

        double new_time = NTime::GetApplicationTime();
        double delta_time = new_time - old_time;
        old_time = new_time;
        char title[64];
        std::snprintf( title, 64, "Wasp -- %.5fms", delta_time * 1000 );
        NWasp::Window::Instance()->SetTitle( title );
    }
    
    NWasp::CameraManager::Destroy();
    NWasp::Scene::Destroy();
    NWasp::Input::Destroy();
    NWasp::EffectManager::Destroy();
    NWasp::StateManager::Destroy();
    NWasp::JfxContext::Destroy();
    NWasp::Window::Destroy();
    
    return 0;
}
