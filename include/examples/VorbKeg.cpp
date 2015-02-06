/************************************************************************/
/* In compilation units, Vorb's PCH must precede any files              */
/* that are part of Vorb.                                               */
/************************************************************************/
#include <Vorb/stdafx.h>

KEG_ENUM_INIT_BEGIN(Color, Color, ke)
using namespace keg;
ke->addValue("Red", Color::RED);
ke->addValue("Orange", Color::ORANGE);
ke->addValue("DoubleRainbow", Color::DOUBLE_RAINBOW);
KEG_TYPE_INIT_END

KEG_TYPE_INIT_BEGIN(Range, Range, kt)
using namespace keg;

KEG_TYPE_INIT_END

KEG_TYPE_INIT_BEGIN(LifetimeRange, Range, kt)
using namespace keg;
kt->addValue("Birth", Value::basic(BasicType::I32, offsetof(Range, min));
kt->addValue("Death", Value::basic(BasicType::I32, offsetof(Range, max));
KEG_TYPE_INIT_END

KEG_TYPE_INIT_BEGIN(Person, Person, kt)
using namespace keg;

KEG_TYPE_INIT_END
