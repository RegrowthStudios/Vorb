#include "Vorb/stdafx.h"
#include "Vorb/io/Keg.h"

#include "Vorb/io/KegFuncs.h"
#include "Vorb/io/KegTypes.h"
#include "Vorb/io/YAML.h"

namespace keg {
#define KEG_BASIC_NUM_MAP(TYPE) \
    { BasicType::TYPE, #TYPE }, \
    {BasicType::TYPE##_V2, #TYPE "_V2"}, \
    {BasicType::TYPE##_V3, #TYPE "_V3"}, \
    {BasicType::TYPE##_V4, #TYPE "_V4"}
    std::map<BasicType, nString> basicTypes = {
        KEG_BASIC_NUM_MAP(I8),
        KEG_BASIC_NUM_MAP(I16),
        KEG_BASIC_NUM_MAP(I32),
        KEG_BASIC_NUM_MAP(I64),
        KEG_BASIC_NUM_MAP(UI8),
        KEG_BASIC_NUM_MAP(UI16),
        KEG_BASIC_NUM_MAP(UI32),
        KEG_BASIC_NUM_MAP(UI64),
        KEG_BASIC_NUM_MAP(F32),
        KEG_BASIC_NUM_MAP(F64),
        { BasicType::BOOL, "Bool" },
        { BasicType::STRING, "String" },
        { BasicType::C_STRING, "CString" }
    };
#undef KEG_BASIC_NUM_MAP

#define KEG_SIMPLE_EVAL(TYPE) \
    evaluate<TYPE>, \
    evaluate<keg::keg##TYPE##v2>, \
    evaluate<keg::keg##TYPE##v3>, \
    evaluate<keg::keg##TYPE##v4>
    Evaluator simpleEvaluators[(size_t)keg::BasicType::COUNT] = {
        KEG_SIMPLE_EVAL(i8),
        KEG_SIMPLE_EVAL(i16),
        KEG_SIMPLE_EVAL(i32),
        KEG_SIMPLE_EVAL(i64),
        KEG_SIMPLE_EVAL(ui8),
        KEG_SIMPLE_EVAL(ui16),
        KEG_SIMPLE_EVAL(ui32),
        KEG_SIMPLE_EVAL(ui64),
        KEG_SIMPLE_EVAL(f32),
        KEG_SIMPLE_EVAL(f64),
        evaluate<bool>,
        evaluate<cString>,
        evaluate<nString>,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };
#undef KEG_SIMPLE_EVAL
#define KEG_SIMPLE_OUT(TYPE) \
    output<TYPE>, \
    output<keg::keg##TYPE##v2>, \
    output<keg::keg##TYPE##v3>, \
    output<keg::keg##TYPE##v4>
    Outputter simpleOutputters[(size_t)keg::BasicType::COUNT] = {
        KEG_SIMPLE_OUT(i8),
        KEG_SIMPLE_OUT(i16),
        KEG_SIMPLE_OUT(i32),
        KEG_SIMPLE_OUT(i64),
        KEG_SIMPLE_OUT(ui8),
        KEG_SIMPLE_OUT(ui16),
        KEG_SIMPLE_OUT(ui32),
        KEG_SIMPLE_OUT(ui64),
        KEG_SIMPLE_OUT(f32),
        KEG_SIMPLE_OUT(f64),
        output<bool>,
        output<cString>,
        output<nString>,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };
#undef KEG_SIMPLE_OUT

    // Our Global Environment :)
    Environment* kegGE = nullptr;
    Environment* getGlobalEnvironment() {
        if (!kegGE) kegGE = new Environment;
        return kegGE;
    }

    Type& getType(bool& initialized, Type& type, bool (*fInit)()) {
        if (initialized) return type;
        initialized = fInit();
        return type;
    }
    Enum& getEnum(bool& initialized, Enum& type, bool (*fInit)()) {
        if (initialized) return type;
        initialized = fInit();
        return type;
    }
}
