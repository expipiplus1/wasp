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

#include "compiler.hpp"

#include <iostream>
#include <string>
#include <aiMesh.h>
#include <aiPostProcess.h>
#include <aiScene.h>
#include <assimp.hpp>
#include <joemath/joemath.hpp>
#include <joefile/output_stream.hpp>

using namespace JoeMath;
using namespace JoeFile;

const u32 WASP_FILE_MAGIC = 0x57415350;
const u32 WASP_FILE_VERSION = 1;
const std::string default_effect_name = "data/effects/phong.cgfx";

namespace NWaspModelCompiler
{
    Compiler::Compiler          ( )
    {
    }

    Compiler::~Compiler         ( )
    {
    }

    bool Compiler::Compile(const std::string source_filename, const std::string dest_filename) const
    {
        Assimp::Importer importer;
        
        u32 flags = aiProcess_CalcTangentSpace |
                    aiProcess_JoinIdenticalVertices |
                    aiProcess_Triangulate |
                    aiProcess_GenSmoothNormals |
                    aiProcess_PreTransformVertices |
                    aiProcess_ValidateDataStructure |
                    aiProcess_ImproveCacheLocality |
                    aiProcess_RemoveRedundantMaterials |
                    aiProcess_FixInfacingNormals |
                    aiProcess_SortByPType |
                    aiProcess_FindDegenerates |
                    aiProcess_FindInvalidData |
                    aiProcess_TransformUVCoords |
                    aiProcess_OptimizeMeshes |
                    aiProcess_OptimizeGraph |
                    aiProcess_FlipWindingOrder;
            
        const aiScene* scene = importer.ReadFile( source_filename.c_str(), flags );
        
        if( !scene->HasMeshes() )
            return false;
        
        OutputStream output_stream;
        
        output_stream << WASP_FILE_MAGIC;
        output_stream << WASP_FILE_VERSION;
        
        output_stream << scene->mNumMeshes;
        
        for( u32 i = 0; i < scene->mNumMeshes; ++i )
        {
            const aiMesh* mesh = scene->mMeshes[i];
            
            output_stream << (u32)0; //Mesh type
            output_stream << mesh->mMaterialIndex; //effect index
            
            output_stream << ( mesh->HasPositions() ? (u32) 1 : (u32) 0 );
            output_stream << ( mesh->HasNormals()   ? (u32) 1 : (u32) 0 );
            output_stream << ( mesh->HasTangentsAndBitangents() ? (u32) 1 : (u32) 0 );
            output_stream << ( mesh->HasTangentsAndBitangents() ? (u32) 1 : (u32) 0 );
            output_stream << ( mesh->GetNumUVChannels() );
            output_stream << ( mesh->GetNumColorChannels() );
            
            output_stream << mesh->mNumVertices;
            output_stream << mesh->mNumFaces * 3;
            
            for( u32 vertex_index = 0; vertex_index < mesh->mNumVertices; ++vertex_index )
            {
                if( mesh->HasPositions() )
                    output_stream << mesh->mVertices[vertex_index];
                
                if( mesh->HasNormals() )
                    output_stream << mesh->mNormals[vertex_index];
                
                if( mesh->HasTangentsAndBitangents() )
                {
                    output_stream << mesh->mTangents[vertex_index];
                    output_stream << mesh->mBitangents[vertex_index];
                }
                
                for( u32 uv_channel_index = 0; uv_channel_index < mesh->GetNumUVChannels(); ++uv_channel_index )
                {
                    output_stream << mesh->mTextureCoords[uv_channel_index][vertex_index];
                    output_stream << 0.0f;
                }
                    
                for( u32 color_channel_index = 0; color_channel_index < mesh->GetNumColorChannels(); ++color_channel_index )
                    output_stream << mesh->mColors[color_channel_index][vertex_index];
            }
            
            for( u32 face_index = 0; face_index < mesh->mNumFaces; ++face_index )
            {
                output_stream << mesh->mFaces[face_index].mIndices[0];
                output_stream << mesh->mFaces[face_index].mIndices[1];
                output_stream << mesh->mFaces[face_index].mIndices[2];
            }
        }
        
        // default effect
        output_stream << default_effect_name;
        
        return output_stream.Write( dest_filename );
    }
};
