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
#include <Cg/cg.h>
#include <joemath/joemath.hpp>
#include "camera.hpp"
#include "camera_manager.hpp"
#include "cg_context.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "timer.hpp"
#include "window.hpp"
#include "input.hpp"

using namespace NJoeMath;
using namespace NWasp;

int main (int argc, char** argv)
{
    if ( !Window::Create() )
        return 1;

    if ( !CgContext::Create() )
        return 2;
    
    if ( !Input::Create() )
        return 3;

    if ( !Scene::Create() )
        return 4;

    if ( !CameraManager::Create() )
        return 5;

    Camera camera;

    camera.SetPosition( float3(1.0f, 0.0f, -2.0f) );
    camera.SetTarget( float3(0.0f, 0.35f, 0.0f) );
    camera.SetAspectRatio( 1.0f );
    camera.SetVerticalFov( DegToRad(70.0f) );
    camera.SetNearPlane( 0.01f );
    camera.SetFarPlane( 100.0f );
    camera.Update();

    CameraManager::Instance()->SetCurrentCamera( &camera );
    
    Mesh mesh;

    Scene::Instance()->AddRenderable( &mesh );
    
    NTime::Timer timer;

    while ( !Window::Instance()->IsWindowClosed( ) )
    {      
        timer.Start( );        

        Scene::Instance()->Update();
        Scene::Instance()->Render();     

        Window::Instance()->Swap( );

        timer.Stop( );

        char title[64];
        std::snprintf( title, 64, "Wasp -- %.2f fps", 1.0 / timer.GetElapsedTime() );
        Window::Instance()->SetTitle( title );
    }
    
    Scene::Destroy();
    Input::Destroy();
    CgContext::Destroy();
    Window::Destroy();
    
    return 0;
}
