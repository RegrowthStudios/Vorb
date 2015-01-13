#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Sound_

#include <include/sound/SoundEngine.h>
#include <include/sound/SoundInstance.h>
#include <include/sound/SoundListener.h>
#include <include/sound/SoundResource.h>
#include <include/Vorb.h>

TEST(Doppler) {
    if (vorb::init(vorb::InitParam::SOUND) != vorb::InitParam::SOUND) return false;

    puts("Loading Sound");
    vsound::Engine eng;
    eng.init();
    vsound::Resource sound = eng.loadSound("data/c.ogg");
    vsound::Instance inst = eng.createInstance(sound);
    inst.setLooped(true);
    inst.play();

    vsound::Listener l;
    puts("Playing Sound");
    f32 rot = 0.0f;
    while (rot < 12) {
        l.velocity = l.position;
        l.position.x = cos(rot);
        l.position.z = 4.0f * sin(rot);
        l.velocity = l.position - l.velocity;
        l.velocity *= 20.0f;
        rot += 0.2f;

        eng.update(l);
        Sleep(50);
    }
    eng.freeSound(sound);
    eng.dispose();

    return vorb::dispose(vorb::InitParam::SOUND) == vorb::InitParam::SOUND;
}

TEST(SoundDefaults) {
    if (vorb::init(vorb::InitParam::SOUND) != vorb::InitParam::SOUND) return false;

    vsound::Engine eng;
    eng.init();
    vsound::Resource sound = eng.loadSound("data/Abyss.mp3");
    vsound::Instance inst = eng.createInstance(sound);
    eng.update(vsound::Listener());

    f32v3 v3;
    f32v2 v2;
    printf("Is Playing: %s\n", inst.isPlaying() ? "True" : "False");
    printf("Is Looped:  %s\n", inst.isLooped() ? "True" : "False");
    v3 = inst.getPosition();
    printf("Position:   (%f,%f,%f)\n", v3.x, v3.y, v3.z);
    v3 = inst.getVelocity();
    printf("Velocity:   (%f,%f,%f)\n", v3.x, v3.y, v3.z);
    v2 = inst.getBoundsRadii();
    printf("Radii:      [%f,%f]\n", v2.x, v2.y);
    printf("Volume:     %f\n", inst.getVolume());
    printf("Frequency:  %f\n", inst.getFrequency());
    printf("Length:     %f seconds\n", inst.getLength());
    
    inst.setCursor(inst.getLength() * 0.5f);
    inst.play();
    Sleep(2000);
    
    eng.freeSound(sound);
    eng.dispose();

    return vorb::dispose(vorb::InitParam::SOUND) == vorb::InitParam::SOUND;
}

TEST(Frequency) {
    if (vorb::init(vorb::InitParam::SOUND) != vorb::InitParam::SOUND) return false;

    vsound::Engine eng;
    eng.init();
    vsound::Resource sound = eng.loadSound("data/c.ogg");
    vsound::Instance inst = eng.createInstance(sound);
    inst.setLooped(true);
    inst.play();
    vsound::Listener l;

    f32 initial = inst.getFrequency();

    inst.setFrequency(initial * 1.0f);
    eng.update(l);
    Sleep(1000);

    inst.setFrequency(initial * 2.0f);
    eng.update(l);
    Sleep(1000);

    inst.setFrequency(initial * 0.5f);
    eng.update(l);
    Sleep(1000);

    eng.freeSound(sound);
    eng.dispose();

    return vorb::dispose(vorb::InitParam::SOUND) == vorb::InitParam::SOUND;
}

TEST(Volume) {
    if (vorb::init(vorb::InitParam::SOUND) != vorb::InitParam::SOUND) return false;

    vsound::Engine eng;
    eng.init();
    vsound::Resource sound = eng.loadSound("data/c.ogg");
    vsound::Instance inst = eng.createInstance(sound);
    inst.setLooped(true);
    inst.play();
    vsound::Listener l;

    f32 initial = inst.getVolume();

    inst.setVolume(initial * 1.0f);
    eng.update(l);
    Sleep(1000);

    inst.setVolume(initial * 2.0f);
    eng.update(l);
    Sleep(1000);

    inst.setVolume(initial * 0.5f);
    eng.update(l);
    Sleep(1000);

    eng.freeSound(sound);
    eng.dispose();

    return vorb::dispose(vorb::InitParam::SOUND) == vorb::InitParam::SOUND;
}