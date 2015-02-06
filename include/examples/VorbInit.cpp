#include <Vorb/Vorb.h>

int main() {
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
