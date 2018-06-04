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
fv: [1.0, 3.0, 10.0, -1.0]
)";

struct KT1 {
public:
    i32 x;
    nString y;
    cString other;
    f32v4 fv;
    ui8 z;
};
KEG_TYPE_DECL(KT1);

KEG_TYPE_DEF(KT1, KT1, kt) {
    using namespace keg;

    kt.addValue("x", Value::basic(offsetof(KT1, x), BasicType::I32));
    kt.addValue("y", Value::basic(offsetof(KT1, y), BasicType::STRING));
    kt.addValue("other", Value::basic(offsetof(KT1, other), BasicType::C_STRING));
    kt.addValue("z",     Value::basic(offsetof(KT1, z),     BasicType::UI8));
    kt.addValue("fv", Value::basic(offsetof(KT1, fv), BasicType::F32_V4));
}

TEST(Parse) {
    KT1 data;
    keg::parse(&data, TestKeg1, "KT1");
    // Validate
    if (data.x != 100) return false;
    if (data.y != "Hello World") return false;
    if (strcmp(data.other, "Cool") != 0) return false;
    if (data.z != 0xf0) return false;
    if (data.fv != f32v4(1.0f, 3.0f, 10.0f, -1.0f)) return false;
    delete[] data.other;
    return true;
}
