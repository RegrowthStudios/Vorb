#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Sound_

#include <include/SoundEngine.h>
#include <include/SoundInstance.h>
#include <include/SoundListener.h>
#include <include/SoundResource.h>
#include <include/Vorb.h>

TEST(PlaySound) {
    if (vorb::init(vorb::InitParam::SOUND) != vorb::InitParam::SOUND) return false;

    puts("Loading Sound");
    vsound::Engine eng;
    eng.init();
    vsound::Resource sound = eng.loadSound("data/Abyss.mp3");
    vsound::Instance inst = eng.playInstance(sound);

    vsound::Listener l;
    puts("Playing Sound");
    f32 rot = 0.0f;
    while (inst.isPlaying()) {
        l.velocity = l.position;
        l.position.x = cos(rot);
        l.position.z = 4.0f * sin(rot);
        l.velocity = l.position - l.velocity;
        l.velocity *= 20.0f;
        rot += 0.1f;

        eng.update(l);
        Sleep(50);
    }
    eng.freeSound(sound);
    eng.dispose();

    return vorb::dispose(vorb::InitParam::SOUND) == vorb::InitParam::SOUND;
}