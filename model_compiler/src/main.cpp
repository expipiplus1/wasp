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

#include <iostream>
#include <joemath/joemath.hpp>
#include "output_stream.hpp"

#include "bunny.h"

using namespace NJoeMath;

const u32 WASP_FILE_MAGIC = 0x57415350;
const u32 WASP_FILE_VERSION = 1;

int main (int argc, char** argv)
{
    NWaspModelCompiler::OutputStream output_stream;
    output_stream << WASP_FILE_MAGIC;
    output_stream << WASP_FILE_VERSION;
    
    u32 num_meshes = 1;
    output_stream << num_meshes;
    
    output_stream << (u32)0;
    output_stream << (u32)0;
    
    output_stream << (u32)1;
    output_stream << (u32)1;
    output_stream << (u32)0;
    output_stream << (u32)0;
    output_stream << (u32)0;
    output_stream << (u32)0;
    
    output_stream << (u32)NUM_POINTS;
    output_stream << (u32)(NUM_TRIANGLES * 3);

    for( u32 i = 0; i < NUM_POINTS * 3; i += 3 )
    {
        output_stream << g_bunnyPositions[i+0];
        output_stream << g_bunnyPositions[i+1];
        output_stream << g_bunnyPositions[i+2];
        
        output_stream << g_bunnyNormals[i+0];
        output_stream << g_bunnyNormals[i+1];
        output_stream << g_bunnyNormals[i+2];
    }
    
    for( u32 i = 0; i < NUM_TRIANGLES * 3; i += 1 )
        output_stream << (u32)g_bunnyIndices[i];
        
    output_stream << std::string( "data/effects/phong.cgfx" );
    
    output_stream.Write( "data/models/bunny.joe" );
}
