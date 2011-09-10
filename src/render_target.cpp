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

#include "render_target.hpp"

#include <iostream>
#include <joemath/joemath.hpp>
#include "wasp_gl.hpp"

using namespace NJoeMath;

namespace NWasp
{
    RenderTarget::RenderTarget                  ( GLuint fbo, GLuint render_texture, GLuint render_buffer, int2 size )
    :m_fbo( fbo )
    ,m_renderTexture( render_texture )
    ,m_renderBuffer( render_buffer )
    ,m_size( size )
    {
    }
        
    RenderTarget::~RenderTarget                 ( )
    {
        //TODO should this free the buffers?
    }

    void RenderTarget::Bind() const
    {
        std::cout << "Binding fbo " << m_fbo << " size: " << m_size.x() << ":" << m_size.y() << "\n";

        glBindTexture( GL_TEXTURE_2D, 0 );
        glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );
        glViewport( 0,0, m_size.x(),m_size.y() );
    }
};
