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

#include <GL/GLee.h>
#include <GL/glfw3.h>
#include <joemath/joemath.hpp>
#include "attribute_indices.hpp"
#include "input_stream.hpp"

namespace NWasp
{
    Mesh::Mesh            ( )
    {
    }
    
    Mesh::~Mesh           ( )
    {

    }
    
    bool    Mesh::Load     ( InputStream& input_stream )
    {
        u32 mesh_type;
        input_stream >> mesh_type;
        
        u32 effect_index;
        input_stream >> effect_index;
        
        u32 num_positions;
        u32 num_normals;
        u32 num_tangents;
        u32 num_bitangents;
        u32 num_texcoords;
        u32 num_colors;
        
        input_stream >> num_positions
                     >> num_normals
                     >> num_tangents
                     >> num_bitangents
                     >> num_texcoords
                     >> num_colors;
                     
        u32 num_vertices;
        input_stream >> num_vertices;
        
        u32 num_indices;
        input_stream >> num_indices;
        
        m_numIndices = num_indices;
        
        u32 vertex_size = 0;
        
        vertex_size += num_positions    * sizeof( float ) * 3;
        vertex_size += num_normals      * sizeof( float ) * 3;
        vertex_size += num_tangents     * sizeof( float ) * 3;
        vertex_size += num_bitangents   * sizeof( float ) * 3;
        vertex_size += num_texcoords    * sizeof( float ) * 4;
        vertex_size += num_colors       * sizeof( float ) * 4;
        
        float* vertices = reinterpret_cast<float*>( input_stream.GetData( num_vertices * vertex_size ) );            
        u32*   indices  = reinterpret_cast<u32*  >( input_stream.GetData( num_indices * sizeof( u32 ) ) );
        
        glGenBuffers( 1, &m_vbo );
        glGenBuffers( 1, &m_ibo );
        glGenVertexArrays( 1, &m_vao ); 

        glBindVertexArray( m_vao );
            
        //
        // Load the vbo data
        //
        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBufferData( GL_ARRAY_BUFFER, num_vertices * vertex_size, vertices, GL_STATIC_DRAW );

        //
        // Load the index data
        //
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( u32 ) * num_indices, indices, GL_STATIC_DRAW );

        float* p = 0;
        
        if( num_positions != 0 )
        {
            glVertexAttribPointer( POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, vertex_size, p  );
            glEnableVertexAttribArray( POSITION_INDEX );
            p += 3;
        }
        
        if( num_normals != 0 )
        {
            glVertexAttribPointer( NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, vertex_size, p );
            glEnableVertexAttribArray( NORMAL_INDEX );
            p += 3;
        }
        
        if( num_tangents != 0 )
        {
            glVertexAttribPointer( TANGENT_INDEX, 3, GL_FLOAT, GL_FALSE, vertex_size, p );
            glEnableVertexAttribArray( TANGENT_INDEX );
            p += 3;
        }

        if( num_bitangents != 0 )
        {
            glVertexAttribPointer( BITANGENT_INDEX, 3, GL_FLOAT, GL_FALSE, vertex_size, p );
            glEnableVertexAttribArray( BITANGENT_INDEX );
            p += 3;
        }
        
        for( u32 i = 0; i < num_texcoords; ++i )
        {
            glVertexAttribPointer( TEXCOORD_INDEX + i, 4, GL_FLOAT, GL_FALSE, vertex_size, p );
            glEnableVertexAttribArray( TEXCOORD_INDEX + i );
            p += 4;
        }
        
        for( u32 i = 0; i < num_colors; ++i )
        {
            glVertexAttribPointer( COLOR_INDEX + i, 4, GL_FLOAT, GL_FALSE, vertex_size, p );
            glEnableVertexAttribArray( COLOR_INDEX + i );
            p += 4;
        }
       
        //assert( p == (float*)vertex_size );
        
        glBindVertexArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
        
        return true;
    }
    
    void    Mesh::Render   ( ) const
    {
        glBindVertexArray( m_vao );
        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );

        glDrawElements( GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0 );

        glBindVertexArray( 0 );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }
};
