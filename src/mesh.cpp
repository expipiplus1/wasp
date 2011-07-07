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
#include <GL/GLee.h>
#include <GL/glfw3.h>
#include <joemath/joemath.hpp>
#include "time.hpp"

#include "bunny.h"

namespace NWasp
{
    CMesh::CMesh            ( )
    :m_shader()
    {
        m_shader.Load( "effects/phong.cgfx" );

        glGenBuffers( 1, &m_vbo );
        glGenBuffers( 1, &m_ibo );
        glGenVertexArrays( 1, &m_vao ); 

        glBindVertexArray( m_vao );
        
        //
        // Load the vbo data
        //
        
        float* vertices = new float[NUM_POINTS * 3 * 2];
        for( u32 i = 0; i < NUM_POINTS; ++i)
        {
            vertices[i*3*2 + 0 + 0*3] = g_bunnyPositions[i*3 + 0];
            vertices[i*3*2 + 1 + 0*3] = g_bunnyPositions[i*3 + 1];
            vertices[i*3*2 + 2 + 0*3] = g_bunnyPositions[i*3 + 2];

            vertices[i*3*2 + 0 + 1*3] = g_bunnyNormals[i*3 + 0];
            vertices[i*3*2 + 1 + 1*3] = g_bunnyNormals[i*3 + 1];
            vertices[i*3*2 + 2 + 1*3] = g_bunnyNormals[i*3 + 2];
        }

        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, sizeof(float) * NUM_POINTS * 2 * 3, vertices, GL_STATIC_DRAW );
        delete[] vertices;

        //
        // Load the index data
        //
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * NUM_TRIANGLES * 3, &g_bunnyIndices[0], GL_STATIC_DRAW );

        
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0   );
        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12 );
        glEnableVertexAttribArray( 0 );
        glEnableVertexAttribArray( 1 );

        glBindVertexArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }
    
    CMesh::~CMesh           ( )
    {

    }
    
    void    CMesh::Render   ( ) const
    {   
        float3 camera_position = float3(1.0f, 0.0f, -2.0f);
        float3 camera_target   = float3(0.0f, 0.35f, 0.0f); 

        float4x4 model = RotateZXY<float,4>( 0.0f, NTime::GetApplicationTime() / 2.0, 0.0f )
                       * (Scale( float4(-10.0f, -10.0f, 10.0f, 1.0f) ) 
                       * Translate( float3(0.0f, 1.25f, 0.3f) ) );
                       
        float4x4 view  = View( camera_position, camera_target - camera_position, float3(0.0f, 1.0f, 0.0f));
        float4x4 projection = Projection( DegToRad( 90.0f ), 1.0f, 0.01f, 100.0f );

        float4x4 modelViewProjection = model * view * projection;

        m_shader.SetModelViewProjection( modelViewProjection );
        m_shader.SetModel              ( model );
        m_shader.SetParameterBySemantic( float3(0.1f, 0.1f, 0.1f),  "AMBIENTCOLOR" );
        m_shader.SetParameterBySemantic( float3(0.5f, 0.5f, 1.0f),  "DIFFUSECOLOR" );
        m_shader.SetParameterBySemantic( float3(1.0f, 0.0f, -2.0f), "LIGHTPOSITION" );
        m_shader.SetParameterBySemantic( 52.0f,                     "SPECULAREXPONENT" );
        m_shader.SetParameterBySemantic( camera_position,           "CAMERAPOSITION" );

        m_shader.Bind();

        glBindVertexArray( m_vao );
        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );

        glDrawElements( GL_TRIANGLES, NUM_TRIANGLES * 3, GL_UNSIGNED_INT, 0 );

        glBindVertexArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }
};
