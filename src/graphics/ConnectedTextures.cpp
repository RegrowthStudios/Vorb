#include "Vorb/stdafx.h"
#include "Vorb/graphics/ConnectedTextures.h"

#ifndef VORB_USING_PCH
#include <memory>
#endif // !VORB_USING_PCH

const size_t& vg::ConnectedTextureHelper::getOffsetFull(const size_t& neighbors) {
    return offsetsFull[neighbors];
}
const size_t& vg::ConnectedTextureHelper::getOffsetSmall(const size_t& neighbors) {
    return offsetsSmall[neighbors];
}

void vg::ConnectedTextureHelper::init() {
    memset(offsetsFull, 0, sizeof(offsetsFull));
    offsetsFull[0xFF] = 0;
    offsetsFull[0xEF] = 1;
    offsetsFull[0xEE] = 2;
    offsetsFull[0xFE] = 3;
    offsetsFull[0xEB] = 4;
    offsetsFull[0xFA] = 5;
    offsetsFull[0xAB] = 6;
    offsetsFull[0xEA] = 7;
    offsetsFull[0x8A] = 8;
    offsetsFull[0xA2] = 9;
    offsetsFull[0x28] = 10;
    offsetsFull[0xA] = 11;
    offsetsFull[0xFB] = 12;
    offsetsFull[0xE3] = 13;
    offsetsFull[0xE0] = 14;
    offsetsFull[0xF8] = 15;
    offsetsFull[0xAF] = 16;
    offsetsFull[0xBE] = 17;
    offsetsFull[0xAE] = 18;
    offsetsFull[0xBA] = 19;
    offsetsFull[0x2A] = 20;
    offsetsFull[0xA8] = 21;
    offsetsFull[0xA0] = 22;
    offsetsFull[0x82] = 23;
    offsetsFull[0xBB] = 24;
    offsetsFull[0x83] = 25;
    offsetsFull[0] = 26;
    offsetsFull[0x38] = 27;
    offsetsFull[0xA3] = 28;
    offsetsFull[0xE8] = 29;
    offsetsFull[0x8B] = 30;
    offsetsFull[0xE2] = 31;
    offsetsFull[0x8] = 32;
    offsetsFull[0x2] = 33;
    offsetsFull[0x88] = 34;
    offsetsFull[0x22] = 35;
    offsetsFull[0xBF] = 36;
    offsetsFull[0x8F] = 37;
    offsetsFull[0xE] = 38;
    offsetsFull[0x3E] = 39;
    offsetsFull[0x8E] = 40;
    offsetsFull[0x3A] = 41;
    offsetsFull[0x2E] = 42;
    offsetsFull[0xB8] = 43;
    offsetsFull[0x20] = 44;
    offsetsFull[0x80] = 45;
    offsetsFull[0xAA] = 46;

    memset(offsetsSmall, 0, sizeof(offsetsSmall));
    offsetsSmall[0x1 | 0x8] = 1;
    offsetsSmall[0x8] = 2;
    offsetsSmall[0x0] = 3;
    offsetsSmall[0x1 | 0x4 | 0x8] = 4;
    offsetsSmall[0x1 | 0x2 | 0x8] = 5;
    offsetsSmall[0x1 | 0x2 | 0x4 | 0x8] = 6;
    offsetsSmall[0x4 | 0x8] = 7;
    offsetsSmall[0x1 | 0x2] = 8;
}

size_t vg::ConnectedTextureHelper::offsetsFull[256];
size_t vg::ConnectedTextureHelper::offsetsSmall[32];
