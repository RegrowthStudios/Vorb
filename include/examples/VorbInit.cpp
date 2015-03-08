/************************************************************************/
/* In compilation units, Vorb's PCH must precede any files              */
/* that are part of Vorb.                                               */
/************************************************************************/
#include <Vorb/stdafx.h>

// Important headers
#include <Vorb/Vorb.h>

int main(int argc, char** argv) {
    // Initialize the graphics and IO modules
    vorb::InitParam modules = vorb::InitParam::GRAPHICS | vorb::InitParam::IO;
    vorb::InitParam v = vorb::init(modules);
    if (v != modules) {
        puts("Could not initialize all modules");
        vorb::dispose(v);
        exit(1);
    }
    
    // Do stuff here
    
    // Dispose initialized modules
    v = vorb::dispose(modules);
    if (v != modules) {
        puts("Could not dispose all modules");
        exit(1);
    }
    
    return 0;
}
