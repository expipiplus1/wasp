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

#include "mesh.hpp"

#include <Cg/cg.h>
#include <GL/glfw3.h>
#include <joemath/joemath.hpp>
#include "time.hpp"

namespace NWasp
{
    CMesh::CMesh            ( )
    :m_shader()
    {
        m_shader.Load( "effects/blue.cgfx" );
    }
    
    CMesh::~CMesh           ( )
    {

    }
    
    void    CMesh::Render   ( )
    {   
        float3 camera_position = float3(10.0f, 0.0f, -10.0f);
        float3 camera_target   = float3(0.0f, 0.0f, 0.0f); 

        float4x4 model = RotateZXY<float,4>( NTime::GetApplicationTime(), NTime::GetApplicationTime() / 2, NTime::GetApplicationTime() / 3 );
        float4x4 view  = View( camera_position, camera_target - camera_position, float3(0.0f, 1.0f, 0.0f));
        float4x4 projection = Projection( DegToRad( 90.0f ), 1.0f, 0.01f, 100.0f );

        float4x4 modelViewProjection = model * view * projection;

        m_shader.SetModelViewProjection( modelViewProjection );

        m_shader.Bind();

        glBegin( GL_LINE_STRIP );
            glVertex3f(0,0,0);
            glVertex3f(0.8f,0.8f,0.8f);
        glEnd( );
        
        glBegin(GL_QUADS);

            // Front Face
            glNormal3f(0, 0, 1);
            glVertex3f(-1.0f, -1.0f,  2.0f);
            glVertex3f( 1.0f, -1.0f,  2.0f);
            glVertex3f( 1.0f,  1.0f,  2.0f);
            glVertex3f(-1.0f,  1.0f,  2.0f);
            // Back Face
            glNormal3f(0, 0, -1);
            glVertex3f(-1.0f, -1.0f, -2.0f);
            glVertex3f(-1.0f,  1.0f, -2.0f);
            glVertex3f( 1.0f,  1.0f, -2.0f);
            glVertex3f( 1.0f, -1.0f, -2.0f);
            // Top Face
            glNormal3f(0, 1, 0);
            glVertex3f(-1.0f,  1.0f, -2.0f);
            glVertex3f(-1.0f,  1.0f,  2.0f);
            glVertex3f( 1.0f,  1.0f,  2.0f);
            glVertex3f( 1.0f,  1.0f, -2.0f);
            // Bottom Face
            glNormal3f(0, -1, 0);
            glVertex3f(-1.0f, -1.0f, -2.0f);
            glVertex3f( 1.0f, -1.0f, -2.0f);
            glVertex3f( 1.0f, -1.0f,  2.0f);
            glVertex3f(-1.0f, -1.0f,  2.0f);
            // Right face
            glNormal3f(1, 0, 0);
            glVertex3f( 1.0f, -1.0f, -2.0f);
            glVertex3f( 1.0f,  1.0f, -2.0f);
            glVertex3f( 1.0f,  1.0f,  2.0f);
            glVertex3f( 1.0f, -1.0f,  2.0f);
            // Left Face
            glNormal3f(-1, 0, 0);
            glVertex3f(-1.0f, -1.0f, -2.0f);
            glVertex3f(-1.0f, -1.0f,  2.0f);
            glVertex3f(-1.0f,  1.0f,  2.0f);
            glVertex3f(-1.0f,  1.0f, -2.0f);

        glEnd();
    }
};
