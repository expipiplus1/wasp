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
#include "wasp_gl.hpp"
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "state_manager.hpp"

namespace NWasp
{
    CgContext* CgContext::s_instance = nullptr;
    
    CgContext::CgContext                  ( )
    {
    }

    CgContext::~CgContext                 ( )
    {
    }

    bool            CgContext::Create          ( )
    {
        assert( s_instance == nullptr );
        s_instance = new CgContext();

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
        
        CGstate render_target_state = cgCreateState( s_instance->m_cgContext, "RenderTarget", CG_INT );
        cgSetStateCallbacks( render_target_state, SetStateRenderTarget, ResetStateRenderTarget, ValidateStateRenderTarget );

        CGstate clear_color_state = cgCreateState( s_instance->m_cgContext, "ClearColor", CG_FLOAT4 );
        cgSetStateCallbacks( clear_color_state, SetStateClearColor, ResetStateClearColor, ValidateStateClearColor );

        CGstate clear_state = cgCreateState( s_instance->m_cgContext, "Clear", CG_BOOL );
        cgSetStateCallbacks( clear_state, SetStateClear, ResetStateClear, ValidateStateClear );
        
        CGstate render_scene_state = cgCreateState( s_instance->m_cgContext, "RenderScene", CG_BOOL );
        cgSetStateCallbacks( render_scene_state, SetStateRenderScene, ResetStateRenderScene, ValidateStateRenderScene );

        CGstate render_fullscreen_quad_state = cgCreateState( s_instance->m_cgContext, "RenderFullscreenQuad", CG_BOOL );
        cgSetStateCallbacks( render_fullscreen_quad_state, SetStateRenderFullscreenQuad, ResetStateRenderFullscreenQuad, ValidateStateRenderFullscreenQuad );


        cgGLRegisterStates( s_instance->m_cgContext );
        cgGLSetManageTextureParameters( s_instance->m_cgContext, CG_TRUE );
        cgGLSetDebugMode( CG_TRUE );
        cgSetParameterSettingMode( s_instance->m_cgContext, CG_DEFERRED_PARAMETER_SETTING );
        
        cgSetErrorCallback( CgErrorCallback );

        return true;
    }
    
    CgContext*     CgContext::Instance        ( )
    {
        assert( s_instance != nullptr );
        return s_instance;
    }

    void            CgContext::Destroy         ( )
    {
        assert( s_instance != nullptr );
        delete s_instance;
        s_instance = nullptr;
    }
    
    CGcontext       CgContext::GetCgContext    ( ) const
    {
        return m_cgContext;
    }
    
    //
    // States
    //
    
    //
    // Clear
    //

    CGbool          CgContext::SetStateClear              ( CGstateassignment state_assignment )
    {
        int num_values;
        const CGbool* clear = cgGetBoolStateAssignmentValues( state_assignment, &num_values );
        
        assert( clear != NULL );
            
        StateManager::Instance()->SetClear( *clear == CG_TRUE );
        
        return CG_TRUE;
    }

    CGbool          CgContext::ResetStateClear            ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetClear();
        
        return CG_TRUE;
    }
    
    CGbool          CgContext::ValidateStateClear         ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateClear();
    }
    
    //
    // Clear Color
    //

    CGbool          CgContext::SetStateClearColor         ( CGstateassignment state_assignment )
    {
        int num_values;
        const float4* clear_color = reinterpret_cast<const float4*>( cgGetFloatStateAssignmentValues( state_assignment, &num_values ) );
        
        assert( num_values == 4 );
        assert( clear_color != NULL );
            
        StateManager::Instance()->SetClearColor( *clear_color );
        
        return CG_TRUE;
    }

    CGbool          CgContext::ResetStateClearColor       ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetClearColor();
        
        return CG_TRUE;
    }
    
    CGbool          CgContext::ValidateStateClearColor    ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateClearColor();
    }
    
    //
    // Render target
    //

    CGbool          CgContext::SetStateRenderTarget         ( CGstateassignment state_assignment )
    {
        int num_values = 0;
        const int* render_target = cgGetIntStateAssignmentValues( state_assignment, &num_values );
        
        assert( num_values == 1 );
        assert( render_target != NULL );

        StateManager::Instance()->SetRenderTarget( *render_target );
        
        return CG_TRUE;
    }

    CGbool          CgContext::ResetStateRenderTarget       ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetRenderTarget();
        
        return CG_TRUE;
    }
    
    CGbool          CgContext::ValidateStateRenderTarget    ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateRenderTarget();
    }
    
    //
    // Render scene
    //

    CGbool          CgContext::SetStateRenderScene         ( CGstateassignment state_assignment )
    {
        int num_values = 0;
        const CGbool* render_scene = cgGetBoolStateAssignmentValues( state_assignment, &num_values );
        
        assert( num_values == 1 );
        assert( render_scene != NULL );
        
        StateManager::Instance()->SetRenderScene( *render_scene == CG_TRUE );
        
        return CG_TRUE;
    }

    CGbool          CgContext::ResetStateRenderScene       ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetRenderScene();
        
        return CG_TRUE;
    }
    
    CGbool          CgContext::ValidateStateRenderScene    ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateRenderScene();
    }

    //
    // Render fullscreen quad
    //

    CGbool          CgContext::SetStateRenderFullscreenQuad   ( CGstateassignment state_assignment )
    {
        int num_values = 0;
        const CGbool* render_fullscreen_quad = cgGetBoolStateAssignmentValues( state_assignment, &num_values );
        
        assert( num_values == 1 );
        assert( render_fullscreen_quad != NULL );
        
        StateManager::Instance()->SetRenderFullscreenQuad( *render_fullscreen_quad == CG_TRUE );
        
        return CG_TRUE;
    }

    CGbool          CgContext::ResetStateRenderFullscreenQuad       ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetRenderFullscreenQuad();
        
        return CG_TRUE;
    }
    
    CGbool          CgContext::ValidateStateRenderFullscreenQuad    ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateRenderFullscreenQuad();
    }
    
    //
    // Error handling
    //
    void CgContext::CgErrorCallback()
    {
        CGerror error;
        const char* error_string = cgGetLastErrorString( &error );
        std::cerr << "Cg Error:\n" 
                  << error_string << std::endl;
        const char* error_listing = cgGetLastListing( CgContext::Instance( )->GetCgContext( ) );
        if ( error_listing != nullptr )
            std::cerr << error_listing << std::endl;
    }
};
