#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Keg_

#include <include/io/Keg.h>

const cString TestKeg1 = R"(
x: 100
y: "Hello World"
other: "Cool"
z: 0xf0
)";

struct KT1 {
public:
    i32 x;
    nString y;
    cString other;
    ui8 z;
};
KEG_TYPE_DECL(KT1);

KEG_TYPE_INIT_BEGIN(KT1, KT1, kt)
using namespace keg;
kt->addValue("x", Value::basic(BasicType::I32, offsetof(KT1, x)));
kt->addValue("y", Value::basic(BasicType::STRING, offsetof(KT1, y)));
kt->addValue("other", Value::basic(BasicType::C_STRING, offsetof(KT1, other)));
kt->addValue("z", Value::basic(BasicType::UI8, offsetof(KT1, z)));
KEG_TYPE_INIT_END

TEST(Parse) {
    KT1 data;
    keg::parse(&data, TestKeg1, "KT1");
    return true;
}
