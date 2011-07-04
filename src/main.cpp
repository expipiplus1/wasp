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

#include <cstdio>
#include <iostream>
#include <Cg/cg.h>
#include <joemath/joemath.hpp>
#include "cg_context.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "timer.hpp"
#include "window.hpp"
#include "input.hpp"

using namespace NJoeMath;
using namespace NWasp;

int main (int argc, char** argv)
{
    if ( !CWindow::Create() )
        return 1;

    if ( !CCgContext::Create() )
        return 2;
    
    if ( !CInput::Create() )
        return 3;

    if ( !CScene::Create() )
        return 4;
    
    CMesh mesh;
    
    NTime::CTimer timer;

    while ( !CWindow::Instance()->IsWindowClosed( ) )
    {      
        timer.Start( );
        
        CScene::Instance()->Update();
        CScene::Instance()->Render();
        
        mesh.Render( );
        
        CWindow::Instance()->Swap( );

        timer.Stop( );

        char title[64];
        std::snprintf( title, 64, "Wasp -- %.2f fps", 1.0 / timer.GetElapsedTime() );
        CWindow::Instance()->SetTitle( title );
    }
    
    CScene::Destroy();
    CInput::Destroy();
    CCgContext::Destroy();
    CWindow::Destroy();
    
    return 0;
}
