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

#include "quad.hpp"

#include <iostream>
#include "wasp_gl.hpp"
#include <joemath/joemath.hpp>
#include "attribute_indices.hpp"

namespace NWasp
{
    Quad::Quad            ( )
    {
        u32 num_vertices = 4;
        m_numIndices = 4;
        u32 vertex_size = sizeof( float ) * 4;
        
        float vertices[] = { -1.0f, -1.0f, 0.0f, 0.0f,
                             1.0f, -1.0f,  1.0f, 0.0f,
                             1.0f, 1.0f,   1.0f, 1.0f,
                             -1.0f, 1.0f,  0.0f, 1.0f };

        u32   indices[]  = { 0, 1, 2, 3 };
        
        glGenBuffers( 1, &m_vbo );
        glGenBuffers( 1, &m_ibo );
        glGenVertexArraysWASP( 1, &m_vao ); 

        glBindVertexArrayWASP( m_vao );
            
        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, num_vertices * vertex_size, vertices, GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( u32 ) * m_numIndices, indices, GL_STATIC_DRAW );

        glVertexAttribPointer( POSITION_INDEX, 2, GL_FLOAT, GL_FALSE, vertex_size, reinterpret_cast<void*>( 0 )  );
        glEnableVertexAttribArray( POSITION_INDEX );
        
        glVertexAttribPointer( TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, vertex_size, reinterpret_cast<void*>( sizeof( float ) * 2 ) );
        glEnableVertexAttribArray( TEXCOORD_INDEX );
        
        glBindVertexArrayWASP( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }
    
    Quad::~Quad           ( )
    {

    }
    
    void    Quad::Render   ( ) const
    {
        glBindVertexArrayWASP( m_vao );

        glDrawElements( GL_QUADS, m_numIndices, GL_UNSIGNED_INT, 0 );
    }
};
