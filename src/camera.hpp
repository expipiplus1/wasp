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

#include <joemath/joemath.hpp>

using namespace NJoeMath;

namespace NWasp
{
    class   CCamera
    {
    public:
        CCamera                 ( );
        virtual ~CCamera        ( );

        void            SetPosition         ( const float3& position );
        void            SetTarget           ( const float3& target );
        void            SetVerticalFov      ( const float   fov );
        void            SetAspectRatio      ( const float   aspect );
        void            SetNearPlane        ( const float   near_plane );
        void            SetFarPlane         ( const float   far_plane );

        void            Update              ( );
        
        const float3&   GetPosition         ( ) const;
        const float4x4& GetView             ( ) const;
        const float4x4& GetProjection       ( ) const;
        const float4x4& GetViewProjection   ( ) const;
    private:
        float3      m_position;
        float3      m_target;
        float       m_verticalFov;
        float       m_aspectRatio;
        float       m_nearPlane;
        float       m_farPlane;

        //
        // Matrices
        //
        float4x4    m_view;
        float4x4    m_projection;
        float4x4    m_viewProjection;
    };
};
