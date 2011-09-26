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

#include "effect.hpp"

#include <cassert>
#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include "wasp_gl.hpp"
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include <joemath/joemath.hpp>
#include "attribute_indices.hpp"
#include "cg_context.hpp"
#include "render_target.hpp"
#include "renderable.hpp"
#include "state_manager.hpp"
#include "window.hpp"

using namespace JoeMath;

namespace NWasp
{
    Effect::Effect    ( )
    {
    }

    Effect::~Effect   ( )
    {
    }

    bool            Effect::Load    ( std::string filename )
    {        
        m_filename = filename;
        m_cgEffect = cgCreateEffectFromFile( CgContext::Instance( )->GetCgContext( ), filename.c_str(), nullptr );

        CGtechnique technique = cgGetFirstTechnique( m_cgEffect );

        CGbool valid;
        valid = cgValidateTechnique( technique );
        
        if (!valid)
        {
            std::cerr << "Couldn't validate technique" << std::endl;
            return false;
        }
               
        return true;
    }
    
    bool            Effect::Reload  ( )
    {
        CGeffect old_effect = m_cgEffect;
        if( !Load( m_filename ) )
        {
            m_cgEffect = old_effect;
            return false;
        }
        return true;
    }

    std::string     Effect::GetName ( ) const
    {
        return std::string( cgGetEffectName( m_cgEffect ) );
    }        
    
    //TODO removeme
    void            Effect::Bind    ( ) const
    {
        CGtechnique technique = cgGetFirstTechnique( m_cgEffect );
        CGpass      pass = cgGetFirstPass( technique );
        cgSetPassState( pass );
    }
    
    CGeffect        Effect::GetCgEffect()
    {
        return m_cgEffect;
    }
    
    void            Effect::RenderPrimitive( Renderable* primitive ) const
    {
        CGtechnique technique = cgGetFirstTechnique( m_cgEffect );
        CGpass      pass      = cgGetFirstPass( technique );
        
        while( pass != nullptr )
        {
            cgSetPassState( pass );
            StateManager::Instance()->ApplyState();
            
            primitive->Render();
            
            cgResetPassState( pass );
            pass = cgGetNextPass( pass );
        }
    }

    void            Effect::SetModelViewProjection ( const float4x4& modelViewProjection ) const
    {
        CGparameter param = cgGetEffectParameterBySemantic( m_cgEffect, "MODELVIEWPROJECTION" );
        cgSetMatrixParameterfr( param, reinterpret_cast<const float*>(&modelViewProjection) );
    }

    void            Effect::SetModel               ( const float4x4& model ) const
    {
        CGparameter param = cgGetEffectParameterBySemantic( m_cgEffect, "MODEL" );
        cgSetMatrixParameterfr( param, reinterpret_cast<const float*>(&model) );
    }

    void            Effect::SetParameterBySemantic    ( const float3& v, const char* semantic ) const
    {
        CGparameter param = cgGetEffectParameterBySemantic( m_cgEffect, semantic );
        cgSetParameter3fv( param, reinterpret_cast<const float*>(&v) );
    }

    void            Effect::SetParameterBySemantic    ( const float v, const char* semantic ) const
    {
        CGparameter param = cgGetEffectParameterBySemantic( m_cgEffect, semantic );
        cgSetParameter1f( param, v );
    }

    void            Effect::InitializeConstants       () const
    {
        CGparameter p = cgGetFirstEffectParameter( m_cgEffect );
        while( p != nullptr )
        {
            std::string semantic = cgGetParameterSemantic( p );
            if( semantic == "SCREENSIZE" )
            {
                CGtype type = cgGetParameterType( p );
                assert( type == CG_INT2 );
                int2 size = Window::Instance()->GetSize();
                cgSetParameter2iv( p, reinterpret_cast<int*>( &size ) );
            }
            if( cgGetParameterName( p ) == std::string( "a" ) )
            {
                u8* data = new u8[512];
                for( u32 i = 0; i < 512; ++i )
                    data[i] = i;
                CGbuffer buffer = cgGLCreateBuffer( CgContext::Instance()->GetCgContext(), 512, data, GL_STATIC_DRAW );
                //CGbuffer buffer = cgCreateBuffer( CgContext::Instance()->GetCgContext(), 512, data, CG_BUFFER_USAGE_STATIC_DRAW );
                cgSetEffectParameterBuffer( p, buffer );
                std::cout << semantic << "\n";
            }
            p = cgGetNextParameter( p );
        }
    }

    void            Effect::AllocateBuffers           ()
    {
        //TODO this could probably be moved to scene manager

        std::queue<CGparameter> pending_render_buffers;
        std::queue<CGparameter> pending_render_textures;
        std::queue<CGparameter> pending_fbos;
        std::queue<CGparameter> pending_samplers;
        std::queue<CGparameter> pending_samplers_1D;

        CGparameter p = cgGetFirstEffectParameter( m_cgEffect );
        while( p != nullptr )
        {
            std::string semantic = cgGetParameterSemantic( p );
            CGtype type = cgGetParameterType( p );
            if( type == CG_SAMPLER2D )
                pending_samplers.push( p );  
            if( type == CG_SAMPLER1D )
                pending_samplers_1D.push( p );
            else if( semantic == "RENDERTEXTURE" )
                pending_render_textures.push( p );
            else if( semantic == "RENDERBUFFER" )
                pending_render_buffers.push( p );
            else if( semantic == "FBO" )
                pending_fbos.push( p );

            p = cgGetNextParameter( p );
        }

        while( !pending_render_textures.empty() )
        {
            CGparameter p = pending_render_textures.front();
            pending_render_textures.pop();

            int2 size;
            u32 format;
            bool have_size = false;
            bool have_format = true;

            CGannotation a = cgGetFirstParameterAnnotation( p );
            while( a != nullptr )
            {
                std::string annotation_name = cgGetAnnotationName( a );

                if( annotation_name == "Size" )
                {
                    int num_values;
                    const int* values = cgGetIntAnnotationValues( a, &num_values ); 
                    assert( num_values == 2 );
                    size = int2( values[0], values[1] );
                    have_size = true; 
                }
                else if( annotation_name == "Format" )
                {
                    int num_values;
                    const int* values = cgGetIntAnnotationValues( a, &num_values ); 
                    assert( num_values == 1 );
                    switch( values[0] )
                    {
                        case RGBA32F:
                            format = GL_RGBA32F; 
                            break;
                    }
                    have_format = true;
                }

                a = cgGetNextAnnotation( a );
            }

            assert( have_format && have_size );

            GLuint render_texture;
            glGenTextures( 1, &render_texture );

            glBindTexture( GL_TEXTURE_2D, render_texture );
            glTexImage2D( GL_TEXTURE_2D, 0, format, size.x(), size.y(), 0,
                          GL_RGBA, GL_FLOAT, nullptr );

            cgSetParameter1i( p, render_texture );
            glBindTexture( GL_TEXTURE_2D, 0 );
        }
        
        while( !pending_render_buffers.empty() )
        {
            CGparameter p = pending_render_buffers.front();
            pending_render_buffers.pop(); 
            int2 size;
            u32 format;
            bool have_size = false;
            bool have_format = false;

            CGannotation a = cgGetFirstParameterAnnotation( p );
            while( a != nullptr )
            {
                std::string annotation_name = cgGetAnnotationName( a );

                if( annotation_name == "Size" )
                {
                    int num_values;
                    const int* values = cgGetIntAnnotationValues( a, &num_values ); 
                    assert( num_values == 2 );
                    size = int2( values[0], values[1] );
                    have_size = true; 
                }
                else if( annotation_name == "Format" )
                {
                    int num_values;
                    const int* values = cgGetIntAnnotationValues( a, &num_values ); 
                    assert( num_values == 1 );
                    switch( values[0] )
                    {
                        case DEPTH32:
                            format = GL_DEPTH_COMPONENT32; 
                            break;
                    }
                    have_format = true;
                }

                a = cgGetNextAnnotation( a );
            }

            assert( have_format && have_size );

            GLuint render_buffer;
            glGenRenderbuffersEXT( 1, &render_buffer );
            
            cgSetParameter1i( p, render_buffer );

            glBindRenderbuffer( GL_RENDERBUFFER, render_buffer);
            glRenderbufferStorage( GL_RENDERBUFFER_EXT,
                                   format, size.x(), size.y() );
             
            glBindRenderbuffer( GL_RENDERBUFFER, 0 );
        }
        
        while( !pending_fbos.empty() )
        {
            CGparameter p = pending_fbos.front();
            pending_fbos.pop();
            u32 render_texture = 0;
            u32 render_buffer  = 0;
            int2 size;

            CGannotation a = cgGetFirstParameterAnnotation( p );
            while( a != nullptr )
            {
                std::string annotation_name = cgGetAnnotationName( a );

                if( annotation_name == "RenderTexture" )
                {
                    int num_values;
                    const int* values = cgGetIntAnnotationValues( a, &num_values ); 
                    assert( num_values == 1 );
                    render_texture = values[0];
                }
                else if( annotation_name == "RenderBuffer" )
                {
                    int num_values;
                    const int* values = cgGetIntAnnotationValues( a, &num_values ); 
                    assert( num_values == 1 );
                    render_buffer = values[0];
                }
                else if( annotation_name == "Size" )
                {
                    int num_values;
                    const int* values = cgGetIntAnnotationValues( a, &num_values ); 
                    assert( num_values == 2 );
                    size = int2( values[0], values[1] );
                }

                a = cgGetNextAnnotation( a );
            }

            GLuint fbo;
            glGenFramebuffers(1, &fbo);

            u32 render_target_index = StateManager::Instance()->AddRenderTarget( RenderTarget( fbo, render_texture, render_buffer, size ) );

            cgSetParameter1i( p, render_target_index );

            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glBindRenderbuffer( GL_RENDERBUFFER, render_buffer );
            glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT,
                                       GL_DEPTH_ATTACHMENT_EXT,
                                       GL_RENDERBUFFER_EXT, render_buffer );

            glBindTexture( GL_TEXTURE_2D, render_texture );
            glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT,
                                    GL_COLOR_ATTACHMENT0_EXT,
                                    GL_TEXTURE_2D, render_texture, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindRenderbuffer( GL_RENDERBUFFER, 0 );
            glBindTexture( GL_TEXTURE_2D, 0 );
        }
        
        while( !pending_samplers.empty() )
        {
            CGparameter p = pending_samplers.front();
            pending_samplers.pop();

            u32 texture = 0;

            CGannotation a = cgGetFirstParameterAnnotation( p );
            while( a != nullptr )
            {
                std::string annotation_name = cgGetAnnotationName( a );

                if( annotation_name == "RenderTexture" )
                {
                    int num_values;
                    const int* values = cgGetIntAnnotationValues( a, &num_values ); 
                    assert( num_values == 1 );
                    texture = values[0];
                }

                a = cgGetNextAnnotation( a );
            }

            std::cout << "creating sampler with texture: " << texture << "\n";

            if( texture == 9999 )
            {
                glGenTextures( 1, &texture );

                float* data = new float[100*100*3];
                for( u32 x = 0; x < 100; ++x )
                {
                    for( u32 y = 0; y < 100; ++y )
                    {
                        u32 index = x * 100 + y;
                        float color = (x*x + y*y < 100*100)?100.0f:0.0f;
                        data[index * 3 + 0] = color;
                        data[index * 3 + 1] = color;
                        data[index * 3 + 2] = color;
                    }
                }

                glBindTexture( GL_TEXTURE_2D, texture );

                glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, 100, 100, 0,
                          GL_RGB, GL_FLOAT, data );

                
            }
            std::cout << "setupsampler: " << texture << "\n";
            cgGLSetupSampler( p, texture );
            glBindTexture( GL_TEXTURE_2D, 0 );
            //cgGLSetTextureParameter( p, texture ); 
            //cgSetSamplerState( p );
        }

        while( !pending_samplers_1D.empty() )
        {
            CGparameter p = pending_samplers_1D.front();
            pending_samplers_1D.pop();

            GLuint texture;
            u32 size = 0;
            bool shuffle = false;

            CGannotation a = cgGetFirstParameterAnnotation( p );
            while( a != nullptr )
            {
                std::string annotation_name = cgGetAnnotationName( a );

                if( annotation_name == "Size" )
                {
                    int num_values;
                    const int* values = cgGetIntAnnotationValues( a, &num_values ); 
                    assert( num_values == 1 );
                    size = values[0];
                }
                else if( annotation_name == "Shuffle" )
                {
                    int num_values;
                    const CGbool* values = cgGetBoolAnnotationValues( a, &num_values );
                    assert( num_values == 1 );
                    shuffle = values[0] == CG_TRUE;
                }

                a = cgGetNextAnnotation( a );
            }

            std::cout << "creating 1d texture, size: " << size << "\n";

            glGenTextures( 1, &texture );

            //TODO allow any data type
            unsigned char* data = nullptr;
            if( shuffle )
            {
                data = new unsigned char[size*3];
                for( u32 x = 0; x < size; ++x )
                {
                    data[x] = x;
                }
                std::random_shuffle( data, data + size );
            }

            glBindTexture( GL_TEXTURE_1D, texture );

            glTexImage1D( GL_TEXTURE_1D, 0, GL_R8, size, 0, GL_RED, GL_UNSIGNED_BYTE, data );
                
            std::cout << "setupsampler1: " << texture << "\n";
            cgGLSetupSampler( p, texture );
            glBindTexture( GL_TEXTURE_1D, 0 );
        }
    }
};
