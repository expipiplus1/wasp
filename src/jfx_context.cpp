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

#include "jfx_context.hpp"

#include <cassert>
#include <iostream>
#include "wasp_gl.hpp"
#include <joefx/context.hpp>
#include "state_manager.hpp"

namespace NWasp
{
    JfxContext* JfxContext::s_instance = nullptr;
    
    JfxContext::JfxContext                  ( )
    {
    }

    JfxContext::~JfxContext                 ( )
    {
    }

    bool            JfxContext::Create          ( )
    {
        assert( s_instance == nullptr );
        s_instance = new JfxContext();

        //
        // Initialize the jfx context
        //
        if( !s_instance->m_jfxContext.Init() )
            return false;
       /* 
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
*/

 //       cgGLRegisterStates( s_instance->m_cgContext );
  //      cgGLSetManageTextureParameters( s_instance->m_cgContext, CG_TRUE );
   //     cgGLSetDebugMode( CG_TRUE );
    //    cgSetParameterSettingMode( s_instance->m_cgContext, CG_DEFERRED_PARAMETER_SETTING );
        
     //   cgSetErrorCallback( JfxErrorCallback );

        return true;
    }
    
    JfxContext*     JfxContext::Instance        ( )
    {
        assert( s_instance != nullptr );
        return s_instance;
    }

    void            JfxContext::Destroy         ( )
    {
        assert( s_instance != nullptr );
        delete s_instance;
        s_instance = nullptr;
    }
    
    JoeFx::Context&       JfxContext::GetJfxContext    ( )
    {
        return m_jfxContext;
    }
    
    //
    // States
    //
    
    //
    // Clear
    //
/*
    CGbool          JfxContext::SetStateClear              ( CGstateassignment state_assignment )
    {
        int num_values;
        const CGbool* clear = cgGetBoolStateAssignmentValues( state_assignment, &num_values );

        assert( clear != nullptr );
            
        StateManager::Instance()->SetClear( *clear == CG_TRUE );
        
        return CG_TRUE;
    }

    CGbool          JfxContext::ResetStateClear            ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetClear();
        
        return CG_TRUE;
    }
    
    CGbool          JfxContext::ValidateStateClear         ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateClear();
    }
    
    //
    // Clear Color
    //

    CGbool          JfxContext::SetStateClearColor         ( CGstateassignment state_assignment )
    {
        int num_values;
        const float4* clear_color = reinterpret_cast<const float4*>( cgGetFloatStateAssignmentValues( state_assignment, &num_values ) );
        
        assert( num_values == 4 );
        assert( clear_color != nullptr );
            
        StateManager::Instance()->SetClearColor( *clear_color );
        
        return CG_TRUE;
    }

    CGbool          JfxContext::ResetStateClearColor       ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetClearColor();
        
        return CG_TRUE;
    }
    
    CGbool          JfxContext::ValidateStateClearColor    ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateClearColor();
    }
    
    //
    // Render target
    //

    CGbool          JfxContext::SetStateRenderTarget         ( CGstateassignment state_assignment )
    {
        int num_values = 0;
        const int* render_target = cgGetIntStateAssignmentValues( state_assignment, &num_values );
        
        assert( num_values == 1 );
        assert( render_target != nullptr );

        StateManager::Instance()->SetRenderTarget( *render_target );
        
        return CG_TRUE;
    }

    CGbool          JfxContext::ResetStateRenderTarget       ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetRenderTarget();
        
        return CG_TRUE;
    }
    
    CGbool          JfxContext::ValidateStateRenderTarget    ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateRenderTarget();
    }
    
    //
    // Render scene
    //

    CGbool          JfxContext::SetStateRenderScene         ( CGstateassignment state_assignment )
    {
        int num_values = 0;
        const CGbool* render_scene = cgGetBoolStateAssignmentValues( state_assignment, &num_values );
        
        assert( num_values == 1 );
        assert( render_scene != nullptr );
        
        StateManager::Instance()->SetRenderScene( *render_scene == CG_TRUE );
        
        return CG_TRUE;
    }

    CGbool          JfxContext::ResetStateRenderScene       ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetRenderScene();
        
        return CG_TRUE;
    }
    
    CGbool          JfxContext::ValidateStateRenderScene    ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateRenderScene();
    }

    //
    // Render fullscreen quad
    //

    CGbool          JfxContext::SetStateRenderFullscreenQuad   ( CGstateassignment state_assignment )
    {
        int num_values = 0;
        const CGbool* render_fullscreen_quad = cgGetBoolStateAssignmentValues( state_assignment, &num_values );
        
        assert( num_values == 1 );
        assert( render_fullscreen_quad != nullptr );
        
        StateManager::Instance()->SetRenderFullscreenQuad( *render_fullscreen_quad == CG_TRUE );
        
        return CG_TRUE;
    }

    CGbool          JfxContext::ResetStateRenderFullscreenQuad       ( CGstateassignment state_assignment )
    {
        StateManager::Instance()->ResetRenderFullscreenQuad();
        
        return CG_TRUE;
    }
    
    CGbool          JfxContext::ValidateStateRenderFullscreenQuad    ( CGstateassignment state_assignment )
    {
        return StateManager::Instance()->ValidateRenderFullscreenQuad();
    }
 */   
    //
    // Error handling
    //
    void JfxContext::JfxErrorCallback()
    {
  /*      CGerror error;
        const char* error_string = cgGetLastErrorString( &error );
        std::cerr << "Jfx Error:\n" 
                  << error_string << std::endl;
        const char* error_listing = cgGetLastListing( JfxContext::Instance( )->GetJfxContext( ) );
        if ( error_listing != nullptr )
            std::cerr << error_listing << std::endl;
            */
    }
};
