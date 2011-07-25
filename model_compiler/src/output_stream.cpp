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

#include "output_stream.hpp"

#include <cstring>
#include <string>
#include <joemath/joemath.hpp>

using namespace NJoeMath;

namespace NWaspModelCompiler
{
    OutputStream::OutputStream  ()
    {
        m_maxSize = 4096;
        m_offset = 0;
        m_data = new u8 [m_maxSize];
    }

    OutputStream::~OutputStream ()
    {
        delete[] m_data;
    }
        
    bool    OutputStream::Write ( std::string filename ) const
    {
        FILE* file;
        file = fopen( filename.c_str(), "wb" );
        if( file == nullptr )
            return false;
        if ( fwrite( m_data, m_offset, 1, file ) != 1 )
            return false;
        if ( fclose( file ) != 0 )
            return false;
        return true;
    }

    void    OutputStream::Append    ( const void* data, const u32 length )
    {
        u32 new_size = length + m_offset;
        if( new_size > m_maxSize )
        {
            while( m_maxSize < new_size )
                m_maxSize *= 2;
            void* old_data = m_data;
            m_data = new u8 [m_maxSize];
            std::memcpy( m_data, old_data, m_offset );
        }
        
        std::memcpy( m_data + m_offset, data, length );
        m_offset += length;
    }
    
    template <>
    OutputStream& OutputStream::operator <<   ( const std::string& t )
    {
        u32 size = t.size();
        Append( &size, sizeof( size ) );
        Append( t.c_str(), size );
        return *this;
    }
};
