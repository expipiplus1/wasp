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

#include "input_stream.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <joemath/joemath.hpp>

using namespace NJoeMath;

namespace NWasp
{
    InputStream::InputStream  ( )
    {
        m_data = nullptr;
        m_size = 0;
        m_offset = 0;
    }

    InputStream::~InputStream ()
    {
        if( m_data != nullptr )
            delete[] m_data;
    }
        
    bool    InputStream::Read ( std::string filename )
    {
        FILE* file;
        file = fopen( filename.c_str(), "rb" );
        if( file == nullptr )
            return false;
        
        if( std::fseek(file, 0, SEEK_END) != 0 )
        {
            fclose( file );
            return false;
        }
    
        m_size = ftell( file );
        
        if( std::fseek(file, 0, SEEK_SET) != 0 )
        {
            fclose( file );
            return false;
        }
        
        m_data = new u8 [m_size];
        
        if ( fread( m_data, m_size, 1, file ) != 1 )
        {
            fclose( file );
            return false;
        }
        
        if ( fclose( file ) != 0 )
            return false;
        
        return true;
    }
    
    void* InputStream::GetData( const u32 length )
    {
        void* data = m_data + m_offset;
        m_offset += length;
        assert( m_offset <= m_size );
        return data;
    }
        
    template <>
    InputStream& InputStream::operator >>   ( std::string& t )
    {
        u32 size = *reinterpret_cast<u32*>( GetData( sizeof( u32 ) ) );
        t.assign( reinterpret_cast<char*>( GetData( size ) ), size );
        return *this;
    }
};
