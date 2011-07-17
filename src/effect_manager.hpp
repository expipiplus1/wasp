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

#include <string>
#include <unordered_map>
#include "Cg/cg.h"
#include "effect.hpp"

namespace NWasp
{
    class EffectManager
    {
    private:
                                EffectManager       ( );
                                ~EffectManager      ( );
                                EffectManager       ( const EffectManager&  )                       = delete;
        EffectManager&          operator =          ( const EffectManager&  )                       = delete;

        static EffectManager*   s_instance;
    public:
        static bool             Create              ( );
        static EffectManager*   Instance            ( );
        static void             Destroy             ( );

        //
        // LoadEffect will load an effect from a file
        // If is_scene_effect it will bind all current and future matching parameters to the parameters in the scene effect
        //
        Effect*                 LoadEffect          ( std::string filename, bool is_scene_effect = false );

    private:
        void                    ConnectParameters   ( CGeffect scene, CGeffect material ) const;
        
        std::unordered_map<std::string, Effect> m_effects;
        Effect*                                 m_sceneEffect;
    };
};
