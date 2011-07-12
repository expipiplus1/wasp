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

#include "camera.hpp"

#include <joemath/joemath.hpp>

namespace NWasp
{
    //CCamera* s_currentCamera = nullptr;

    Camera::Camera    ( )
    {
    }

    Camera::~Camera   ( )
    {
    }

    void    Camera::SetPosition        ( const float3&   position )
    {
        m_position = position;
    }

    void    Camera::SetTarget          ( const float3&   target )
    {
        m_target = target;
    }

    void    Camera::SetVerticalFov     ( const float fov )
    {
        m_verticalFov = fov;
    }

    void    Camera::SetAspectRatio     ( const float aspect )
    {
        m_aspectRatio = aspect;
    }

    void    Camera::SetNearPlane       ( const float near_plane )
    {
        m_nearPlane = near_plane;
    }

    void    Camera::SetFarPlane        ( const float far_plane )
    {
        m_farPlane = far_plane;
    }

    void    Camera::Update             ( )
    {
        m_view  = View( m_position, m_target - m_position, float3(0.0f, 1.0f, 0.0f));
        m_projection = Projection( m_verticalFov, m_aspectRatio, m_nearPlane, m_farPlane );
        m_viewProjection = m_view * m_projection;
    }

    const float3&       Camera::GetPosition         ( ) const
    {
        return m_position;
    }

    const float4x4&     Camera::GetView             ( ) const
    {
        return m_view;
    }
    
    const float4x4&     Camera::GetProjection       ( ) const
    {
        return m_projection;
    }

    const float4x4&     Camera::GetViewProjection   ( ) const
    {
        return m_viewProjection;
    }
};
