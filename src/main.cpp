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
#include "mesh.hpp"
#include "shader.hpp"
#include "shader_buffer.hpp"
#include "window.hpp"
#include "timer.hpp"

using namespace NJoeMath;
using namespace NWasp;

int main (int argc, char** argv)
{
    CWindow window;
    if ( !window.Init() )
        return 1;
    
    CShader red_shader;
    red_shader.Load( "shaders/red.cgfx" );
    
    CMesh mesh;
    
    SCameraMatrixBuffer camera_buffer;
    float4x4 projection = Projection( DegToRad( 90.0f ), 1.0f, 0.01f, 100.0f );
    float4x4 model = Translate( float3(0.0f, 1.0f, 0.0f) );
    float3 camera_position = float3(10.0f, 0.0f, -10.0f);
    float3 camera_target   = float3(0.0f, 0.0f, 0.0f); 
    float4x4 view  = View( camera_position, camera_target - camera_position, float3(0.0f, 1.0f, 0.0f));
    camera_buffer.m_modelView = model * view;
    camera_buffer.m_modelViewProjection = camera_buffer.m_modelView * projection;
    
    CCameraBuffer::Instance()->Set( camera_buffer );
    CCameraBuffer::Instance()->Update( );
    
    NTimer::CTimer timer;

    while ( !window.IsWindowClosed( ) )
    {      
        timer.Start( );
        
        red_shader.Bind( );
        
        mesh.Render( );
        
        window.Swap( );

        timer.Stop( );

        char title[64];
        std::snprintf( title, 64, "Wasp -- %.2f fps", 1.0 / timer.GetElapsedTime() );
        window.SetTitle( title );
    }
    
    return 0;
}
