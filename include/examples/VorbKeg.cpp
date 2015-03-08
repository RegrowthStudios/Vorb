/************************************************************************/
/* In compilation units, Vorb's PCH must precede any files              */
/* that are part of Vorb.                                               */
/************************************************************************/
#include <Vorb/stdafx.h>

KEG_ENUM_DEF(Color, Color, ke) {
    using namespace keg;
    ke.addValue("Red", Color::RED);
    ke.addValue("Orange", Color::ORANGE);
    ke.addValue("DoubleRainbow", Color::DOUBLE_RAINBOW);
}

KEG_TYPE_DEF(Range, Range, kt) {
    using namespace keg;
    kt.addValue("Min", Value::basic(offsetof(Range, min), BasicType::I32);
    kt.addValue("Max", Value::basic(offsetof(Range, max), BasicType::I32);
}

KEG_TYPE_DEF(LifetimeRange, Range, kt) {
    using namespace keg;
    kt.addValue("Birth", Value::basic(offsetof(Range, min), BasicType::I32);
    kt.addValue("Death", Value::basic(offsetof(Range, max), BasicType::I32);
}

KEG_TYPE_DEF(Person, Person, kt) {
    using namespace keg;
    kt.addValue("Name", Value::basic(offsetof(Person, name), BasicType::STRING);
    kt.addValue("IsMan", Value::basic(offsetof(Person, isMan), BasicType::BOOL);
    kt.addValue("FavoriteColor", Value::custom(offsetof(Person, favoriteColor), "Color", true));
    kt.addValue("Lifetime", Value::custom(offsetof(Person, lifetime), "LifetimeRange"));
    kt.addValue("MySecret", Value::ptr(offsetof(Person, secret), BasicType::CUSTOM));
}