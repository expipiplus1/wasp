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

#include "cg_context.hpp"

#include <cassert>
#include <iostream>
#include <Cg/cg.h>
#include <Cg/cgGL.h>

namespace NWasp
{
    CCgContext* CCgContext::s_instance = nullptr;
    
    CCgContext::CCgContext                  ( )
    {
    }

    CCgContext::~CCgContext                 ( )
    {
    }

    bool            CCgContext::Create          ( )
    {
        assert( s_instance == nullptr );
        s_instance = new CCgContext();

        //
        // Initialize the cg context
        //
        s_instance->m_cgContext = cgCreateContext( );
        
        CGerror error;
        const char* error_string = cgGetLastErrorString(&error);
        if (error != CG_NO_ERROR)
        {
            std::cerr << "Cg Error creating context\n"
                      << error_string << std::endl;
            return false;
        }
        
        CGstate render_scene_state = cgCreateState( s_instance->m_cgContext, "RenderScene", CG_BOOL );
        cgSetStateCallbacks( render_scene_state, StateRenderSceneSet, StateRenderSceneReset, StateRenderSceneValidate );

        cgGLRegisterStates( s_instance->m_cgContext );
        cgGLSetManageTextureParameters( s_instance->m_cgContext, CG_TRUE );
        cgGLSetDebugMode( CG_TRUE );
        cgSetParameterSettingMode( s_instance->m_cgContext, CG_DEFERRED_PARAMETER_SETTING );
        
        cgSetErrorCallback( CgErrorCallback );

        return true;
    }
    
    CCgContext*     CCgContext::Instance        ( )
    {
        assert( s_instance != nullptr );
        return s_instance;
    }

    void            CCgContext::Destroy         ( )
    {
        assert( s_instance != nullptr );
        delete s_instance;
        s_instance = nullptr;
    }
    
    CGcontext       CCgContext::GetCgContext    ( ) const
    {
        return m_cgContext;
    }
    
    //
    // States
    //
    
    CGbool          CCgContext::StateRenderSceneSet         ( CGstateassignment state_assignment )
    {
        int num_values = 0;
        const CGbool* render_scene = cgGetBoolStateAssignmentValues( state_assignment, &num_values );
        std::cout << "RenderScene called with " << *render_scene << "\n";
        return CG_TRUE;
    }

    CGbool          CCgContext::StateRenderSceneReset       ( CGstateassignment state_assignment )
    {
        std::cout << "Resetting RenderScene\n";
        return CG_TRUE;
    }
    
    CGbool          CCgContext::StateRenderSceneValidate    ( CGstateassignment state_assignment )
    {
        std::cout << "Validating RenderScene\n";
        return CG_TRUE;
    }
    
    //
    // Error handling
    //
    void CCgContext::CgErrorCallback()
    {
        CGerror error;
        const char* error_string = cgGetLastErrorString( &error );
        std::cerr << "Cg Error:\n" 
                  << error_string << std::endl;
        const char* error_listing = cgGetLastListing( CCgContext::Instance( )->GetCgContext( ) );
        if ( error_listing != nullptr )
            std::cerr << error_listing << std::endl;
    }
};
