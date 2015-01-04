#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Sound_

#include <fmod/fmod.hpp>

#include <include/SoundEngine.h>
#include <include/SoundInstance.h>
#include <include/SoundListener.h>
#include <include/SoundResource.h>
#include <include/Vorb.h>

TEST(PlaySound) {
    if (vorb::init(vorb::InitParam::SOUND) != vorb::InitParam::SOUND) return false;

    vsound::Engine eng;
    vsound::Resource sound = eng.loadSound("data/Abyss.mp3");
    vsound::Instance inst = eng.createSound(sound);
    inst.play();

    vsound::Listener l;
    l.position.z = 100;
    while (inst.isPlaying()) {
        eng.update(l);
        Sleep(500);
    }
    eng.freeSound(sound);

    return vorb::dispose(vorb::InitParam::SOUND) == vorb::InitParam::SOUND;
}