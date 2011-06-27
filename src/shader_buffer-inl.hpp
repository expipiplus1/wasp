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

#pragma once

#include "shader_buffer.hpp"

#include <Cg/cg.h>
#include <joemath/joemath.hpp>
#include "cg_context.hpp"

namespace NWasp
{
    template <typename BufferStruct>
    CShaderBuffer<BufferStruct>* CShaderBuffer<BufferStruct>::s_instance = NULL;
    
    template<typename BufferStruct>
    CShaderBuffer<BufferStruct>::CShaderBuffer  ( )
    {
        m_cgBuffer = cgCreateBuffer( CCgContext::Instance( )->GetCgContext( ), sizeof( BufferStruct ), &m_data, CG_BUFFER_USAGE_DYNAMIC_DRAW );
        
        //
        // Check for error
        //
        CGerror error;
        const char* error_string = cgGetLastErrorString(&error);
        if (error != CG_NO_ERROR)
        {
            std::cerr << "Cg Error creating buffer\n"
                      << error_string << std::endl;
        }
        
    }
       
    template <typename BufferStruct>
    CShaderBuffer<BufferStruct>*    CShaderBuffer<BufferStruct>::Instance   ( )
    {
        if( s_instance == NULL )
            s_instance = new CShaderBuffer<BufferStruct>( );
        return s_instance;
    }
    
    template <typename BufferStruct>
    void                            CShaderBuffer<BufferStruct>::Set        ( const BufferStruct& data )
    {
        m_data = data;
    }
    
    template <typename BufferStruct>
    const BufferStruct&             CShaderBuffer<BufferStruct>::Get        ( ) const
    {
        return m_data;
    }
    
    template <typename BufferStruct>
    void                            CShaderBuffer<BufferStruct>::Update     ( ) const
    {
        cgSetBufferData( m_cgBuffer, sizeof( BufferStruct ), &m_data );
    }
    
    template <typename BufferStruct>
    CGbuffer                        CShaderBuffer<BufferStruct>::GetCgBuffer( ) const
    {
        return m_cgBuffer;
    }
    
};