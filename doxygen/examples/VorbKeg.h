#pragma once
#ifndef VorbKeg_h__
#define VorbKeg_h__

#include <Vorb/io/Keg.h>

enum Color {
    RED,
    ORANGE,
    DOUBLE_RAINBOW
};
KEG_ENUM_DECL(Color);

struct Range {
    i32 min;
    i32 max;
};
KEG_TYPE_DECL(Range);
KEG_TYPE_DECL(LifetimeRange);

class Person {
public:
    nString name;
    bool isMan;
    Color favoriteColor;
    Range lifetime;
private:
    void* secrets;
};
KEG_TYPE_DECL(Person);

#endif // VorbKeg_h__
