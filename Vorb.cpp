#include "stdafx.h"
#include "Vorb.h"

namespace vorb {
    InitParam initGraphics() {
        return InitParam::GRAPHICS;
    }
    InitParam initIO() {
        return InitParam::IO;
    }
    InitParam initSound() {
        return InitParam::SOUND;
    }
}

vorb::InitParam vorb::init(const InitParam& p) {
#define HAS(v, b) ((v & b) != InitParam::NONE)

    vorb::InitParam succeeded;
    if (HAS(p, InitParam::SOUND)) succeeded |= initSound();
    if (HAS(p, InitParam::GRAPHICS)) succeeded |= initGraphics();
    if (HAS(p, InitParam::IO)) succeeded |= initGraphics();
    return succeeded;

#undef HAS
}
