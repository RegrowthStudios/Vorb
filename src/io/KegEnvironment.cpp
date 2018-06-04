#include "stdafx.h"
#include "io/KegEnvironment.h"

#include "io/KegBasic.h"
#include "io/KegTypes.h"

keg::Environment::Environment() :
    _uuid(KEG_BAD_TYPE_ID) {
    Value kv;
    Type kt;

#define NUM_TYPE(TYPE, C_TYPE, NUM) \
    _internalTypes[(i32)BasicType::TYPE].setStructType<C_TYPE>(); \
    kv = Value::basic(sizeof(C_TYPE)* 0, BasicType::TYPE); \
    _internalTypes[(i32)BasicType::TYPE].addValue("value0", kv); \
    _internalTypes[(i32)BasicType::TYPE].addValue("x", kv); \
    _internalTypes[(i32)BasicType::TYPE].addValue("r", kv); \
    _internalTypes[(i32)BasicType::TYPE].addValue("s", kv); \
    _internalTypes[(i32)BasicType::TYPE].addValue("0", kv); \
    addType(#TYPE, &_internalTypes[(i32)BasicType::TYPE]); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addSuper(&_internalTypes[(i32)BasicType::TYPE]); \
    _internalTypes[(i32)BasicType::TYPE##_V2].setStructType<keg##C_TYPE##v2>(); \
    kv = Value::basic(sizeof(C_TYPE)* 1, BasicType::TYPE); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("value1", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("y", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("g", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("t", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("1", kv); \
    addType(#TYPE"_V2", &_internalTypes[(i32)BasicType::TYPE##_V2]); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addSuper(&_internalTypes[(i32)BasicType::TYPE##_V2]); \
    _internalTypes[(i32)BasicType::TYPE##_V3].setStructType<keg##C_TYPE##v3>(); \
    kv = Value::basic(sizeof(C_TYPE)* 2, BasicType::TYPE); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("value2", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("z", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("b", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("p", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("2", kv); \
    addType(#TYPE"_V3", &_internalTypes[(i32)BasicType::TYPE##_V3]); \
    _internalTypes[(i32)BasicType::TYPE##_V4].addSuper(&_internalTypes[(i32)BasicType::TYPE##_V3]); \
    _internalTypes[(i32)BasicType::TYPE##_V4].setStructType<keg##C_TYPE##v4>(); \
    kv = Value::basic(sizeof(C_TYPE)* 3, BasicType::TYPE); \
    _internalTypes[(i32)BasicType::TYPE##_V4].addValue("value3", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V4].addValue("w", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V4].addValue("a", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V4].addValue("q", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V4].addValue("3", kv); \
    addType(#TYPE"_V4", &_internalTypes[(i32)BasicType::TYPE##_V4]); \
    kv = Value::basic(sizeof(C_TYPE)* 0, BasicType::TYPE); \
    _internalTypes[(i32)BasicType::TYPE].addValue("value", kv); \
    kv = Value::basic(sizeof(C_TYPE)* 0, BasicType::TYPE##_V2); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("value", kv); \
    kv = Value::basic(sizeof(C_TYPE)* 0, BasicType::TYPE##_V3); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("value", kv); \
    kv = Value::basic(sizeof(C_TYPE)* 0, BasicType::TYPE##_V4); \
    _internalTypes[(i32)BasicType::TYPE##_V4].addValue("value", kv) \

    NUM_TYPE(I8, i8, 0);
    NUM_TYPE(I16, i16, 4);
    NUM_TYPE(I32, i32, 8);
    NUM_TYPE(I64, i64, 12);
    NUM_TYPE(UI8, ui8, 16);
    NUM_TYPE(UI16, ui16, 20);
    NUM_TYPE(UI32, ui32, 24);
    NUM_TYPE(UI64, ui64, 28);
    NUM_TYPE(F32, f32, 32);
    NUM_TYPE(F64, f64, 36);

    _internalTypes[(i32)BasicType::C_STRING].setStructType<cString>();
    kv = Value::basic(0, BasicType::C_STRING);
    _internalTypes[(i32)BasicType::C_STRING].addValue("value", kv);
    _internalTypes[(i32)BasicType::C_STRING].addValue("string", kv);
    addType("CString", &_internalTypes[(i32)BasicType::C_STRING]);

    _internalTypes[(i32)BasicType::STRING].setStructType<nString>();
    kv = Value::basic(0, BasicType::STRING);
    _internalTypes[(i32)BasicType::STRING].addValue("value", kv);
    _internalTypes[(i32)BasicType::STRING].addValue("string", kv);
    addType("String", &_internalTypes[(i32)BasicType::STRING]);

    _internalTypes[(i32)BasicType::BOOL].setStructType<bool>();
    kv = Value::basic(0, BasicType::BOOL);
    _internalTypes[(i32)BasicType::BOOL].addValue("value", kv);
    _internalTypes[(i32)BasicType::BOOL].addValue("bool", kv);
    addType("Bool", &_internalTypes[(i32)BasicType::BOOL]);

    _internalTypes[(i32)BasicType::ARRAY].setStructType<ArrayBase>();
    kv = Value::array(0, Value::custom(0, "", 0));
    _internalTypes[(i32)BasicType::ARRAY].addValue("value", kv);
    _internalTypes[(i32)BasicType::ARRAY].addValue("array", kv);
    _internalTypes[(i32)BasicType::ARRAY].addValue("set", kv);
    _internalTypes[(i32)BasicType::ARRAY].addValue("list", kv);
    _internalTypes[(i32)BasicType::ARRAY].addValue("elements", kv);
    _internalTypes[(i32)BasicType::ARRAY].addValue("data", kv);
    addType("Array", &_internalTypes[(i32)BasicType::ARRAY]);
}

ui32 keg::Environment::addType(const nString& name, Type* type) {
    _uuid++;
    _typesByName[name] = type;
    _typesByID[_uuid] = type;
    return _uuid;
}
ui32 keg::Environment::addEnum(const nString& name, Enum* type) {
    _uuid++;
    _enumsByName[name] = type;
    _enumsByID[_uuid] = type;
    return _uuid;
}
