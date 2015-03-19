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

KEG_TYPE_DEF(KT1, KT1, kt) {
    using namespace keg;
    kt.addValue("x",     Value::basic(offsetof(KT1, x),     BasicType::I32));
    kt.addValue("y",     Value::basic(offsetof(KT1, y),     BasicType::STRING));
    kt.addValue("other", Value::basic(offsetof(KT1, other), BasicType::C_STRING));
    kt.addValue("z",     Value::basic(offsetof(KT1, z),     BasicType::UI8));
}

TEST(Parse) {
    KT1 data;
    keg::parse(&data, TestKeg1, "KT1");
    delete[] data.other;
    return true;
}
