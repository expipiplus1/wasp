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

#include <Cg/cg.h>

namespace NWasp
{
    class CgContext
    {
    private:
                            CgContext                   ( );
                            ~CgContext                  ( );
                            CgContext                   ( const CgContext&   )                          = delete;
        CgContext&          operator =                  ( const CgContext&   )                          = delete;
        
        static CgContext*   s_instance;
    public: 
        //
        // Singleton functions
        //
        static bool         Create                      ( );
        static CgContext*   Instance                    ( );
        static void         Destroy                     ( );
        
        //
        // Callbacks
        //
        static void         CgErrorCallback             ( );

        //
        // Getters
        //
        CGcontext           GetCgContext                ( ) const;

    private:
        //
        // States
        //
        
        static CGbool       SetStateClearColor         ( CGstateassignment state_assignment );
        static CGbool       ResetStateClearColor       ( CGstateassignment state_assignment );
        static CGbool       ValidateStateClearColor    ( CGstateassignment state_assignment );
        
        static CGbool       SetStateClear         ( CGstateassignment state_assignment );
        static CGbool       ResetStateClear       ( CGstateassignment state_assignment );
        static CGbool       ValidateStateClear    ( CGstateassignment state_assignment );
        
        static CGbool       SetStateRenderTarget         ( CGstateassignment state_assignment );
        static CGbool       ResetStateRenderTarget       ( CGstateassignment state_assignment );
        static CGbool       ValidateStateRenderTarget    ( CGstateassignment state_assignment );
        
        static CGbool       SetStateRenderScene         ( CGstateassignment state_assignment );
        static CGbool       ResetStateRenderScene       ( CGstateassignment state_assignment );
        static CGbool       ValidateStateRenderScene    ( CGstateassignment state_assignment );
        
        static CGbool       SetStateRenderFullscreenQuad         ( CGstateassignment state_assignment );
        static CGbool       ResetStateRenderFullscreenQuad       ( CGstateassignment state_assignment );
        static CGbool       ValidateStateRenderFullscreenQuad    ( CGstateassignment state_assignment );
        
    private:
        CGcontext           m_cgContext;
    };
};
