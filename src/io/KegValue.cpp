#include "stdafx.h"
#include "io/KegValue.h"

#include "io/KegBasic.h"

keg::Value keg::Value::basic(size_t off, BasicType t) {
    Value kv = {};
    kv.type = t;
    kv.offset = off;
    kv.typeName.clear();
    kv.evaluator = simpleEvaluators[(size_t)t];
    kv.outputter = simpleOutputters[(size_t)t];
    return std::move(kv);
}
keg::Value keg::Value::custom(size_t off, const nString& t, bool isEnum /*= false*/) {
    Value kv = {};
    kv.type = isEnum ? BasicType::ENUM : BasicType::CUSTOM;
    kv.offset = off;
    kv.typeName = t;
    kv.evaluator = nullptr;
    kv.outputter = nullptr;
    return std::move(kv);
}
keg::Value keg::Value::array(size_t off, const Value& interior) {
    Value kv = {};
    kv.type = BasicType::ARRAY;
    kv.offset = off;
    kv.typeName.clear();
    kv.interiorValue.reset(new Value(interior));
    kv.evaluator = nullptr;
    kv.outputter = nullptr;
    return std::move(kv);
}
keg::Value keg::Value::array(size_t off, const BasicType& t) {
    return array(off, Value::basic(0, t));
}
keg::Value keg::Value::ptr(size_t off, const Value& interior) {
    Value kv = {};
    kv.type = BasicType::PTR;
    kv.offset = off;
    kv.typeName.clear();
    kv.interiorValue.reset(new Value(interior));
    kv.evaluator = nullptr;
    kv.outputter = nullptr;
    return std::move(kv);
}
keg::Value keg::Value::ptr(size_t off, const BasicType& t) {
    return ptr(off, Value::basic(0, t));
}
