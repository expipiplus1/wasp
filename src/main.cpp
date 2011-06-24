#include <iostream>

#include "window.hpp"

int main (int argc, char** argv)
{
    CWindow window;
    if ( !window.Init() )
        return 1;
    
    while ( !window.IsWindowClosed( ) )
    {      
        window.Swap( );
    }
    
    return 0;
}
