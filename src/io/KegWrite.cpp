#include "stdafx.h"
#include "io/Keg.h"

#include "io/YAML.h"
#include "io/KegFuncs.h"

namespace keg {
    bool write(const ui8* src, keg::YAMLWriter& e, Environment* env, Type* type);

    nString write(const void* src, Type* type, Environment* env /*= nullptr*/) {
        // Test Arguments
        if (env == nullptr) env = getGlobalEnvironment();
        if (src == nullptr || type == nullptr) {
            return nullptr;
        }

        keg::YAMLWriter e;
        e.push(keg::WriterParam::BEGIN_MAP);
        if (!write((ui8*)src, e, env, type)) {
            return nullptr;
        }
        e.push(keg::WriterParam::END_MAP);
        return nString(e.c_str());
    }
    nString write(const void* src, const nString& typeName, Environment* env /*= nullptr*/) {
        // Test Arguments
        if (env == nullptr) env = getGlobalEnvironment();
        if (src == nullptr || typeName.empty()) {
            return nullptr;
        }

        // Attempt To Find The Type
        Type* type = env->getType(typeName);
        if (type == nullptr) return nullptr;

        keg::YAMLWriter e;
        e.push(keg::WriterParam::BEGIN_MAP);
        if (!write((ui8*)src, e, env, type)) {
            return "";
        }
        e.push(keg::WriterParam::END_MAP);
        return nString(e.c_str());
    }
    nString write(const void* src, const ui32& typeID, Environment* env /*= nullptr*/) {
        // Test Arguments
        if (env == nullptr) env = getGlobalEnvironment();
        if (src == nullptr || typeID == KEG_BAD_TYPE_ID) {
            return nullptr;
        }

        // Attempt To Find The Type
        Type* type = env->getType(typeID);
        if (type == nullptr) return nullptr;

        keg::YAMLWriter e;
        e.push(keg::WriterParam::BEGIN_MAP);
        if (!write((ui8*)src, e, env, type)) {
            return nullptr;
        }
        e.push(keg::WriterParam::END_MAP);
        return nString(e.c_str());
    }

    bool write(const ui8* src, keg::YAMLWriter& e, Environment* env, Type* type) {
        // TODO: Add Ptr And Array Support

        Type* interiorType = nullptr;
        Enum* interiorEnum = nullptr;

        auto iter = type->getIter();
        while (iter != type->getIterEnd()) {
            // Write The Key
            e.push(keg::WriterParam::KEY) << iter->first;
            e.push(keg::WriterParam::VALUE);

            // Write The Value
            Value v = iter->second;
            const ui8* data = src + v.offset;

            if (v.outputter) {
                v.outputter(e, data);
            } else {
                switch (v.type) {
                case BasicType::ENUM:
                    // Attempt To Find The Enum
                    interiorEnum = env->getEnum(v.typeName);
                    if (interiorEnum == nullptr) {
                        return false;
                    }
                    // Write Enum String
                    e << interiorEnum->getValue(data);
                    break;
                case BasicType::CUSTOM:
                    // Attempt To Find The Type
                    interiorType = env->getType(v.typeName);
                    if (interiorType == nullptr) {
                        return false;
                    }
                    // Write To Interior Node
                    e.push(keg::WriterParam::BEGIN_MAP);
                    write(data, e, env, interiorType);
                    e.push(keg::WriterParam::END_MAP);
                    break;
                case BasicType::PTR:
                    break;
                case BasicType::ARRAY:
                    // Attempt To Find The Type
                    interiorType = env->getType(v.interiorValue->typeName);
                    if (interiorType == nullptr) return false;

                    // TODO: Write To Interior Array
                    //writeArray(*(ArrayBase*)data, e, env, interiorType);
                    break;
                case BasicType::BOOL:
                    e << *(bool*)data;
                    break;
                case BasicType::C_STRING:
                    e << *(cString*)data;
                    break;
                case BasicType::STRING:
                    e << (*(nString*)data);
                    break;

                    // For when we want to cast TYPE to C_TYPE
#define EMIT_CAST(TYPE, C_TYPE) \
                case BasicType::TYPE: e << (C_TYPE)*data; break; \
                case BasicType::TYPE##_V2: e << keg##C_TYPE##v2(data); break; \
                case BasicType::TYPE##_V3: e << keg##C_TYPE##v3(data); break; \
                case BasicType::TYPE##_V4: e << keg##C_TYPE##v4(data); break
                    // For when we want to interpret TYPE as C_TYPE
#define EMIT_NUM(TYPE, C_TYPE) \
                case BasicType::TYPE: e << *(C_TYPE*)data; break; \
                case BasicType::TYPE##_V2: e << *(keg##C_TYPE##v2*)data; break; \
                case BasicType::TYPE##_V3: e << *(keg##C_TYPE##v3*)data; break; \
                case BasicType::TYPE##_V4: e << *(keg##C_TYPE##v4*)data; break
                    EMIT_CAST(I8, i32); // Prints out bytes as ints
                    EMIT_NUM(I16, i16);
                    EMIT_NUM(I32, i32);
                    EMIT_NUM(I64, i64);
                    EMIT_CAST(UI8, ui32); // Prints out bytes as ints
                    EMIT_NUM(UI16, ui16);
                    EMIT_NUM(UI32, ui32);
                    EMIT_NUM(UI64, ui64);
                    EMIT_NUM(F32, f32);
                    EMIT_NUM(F64, f64);
                default:
                    break;
                }
            }
            iter++;
        }
        return true;
    }
    bool writeArray(ArrayBase, keg::YAMLWriter& e, Environment*, Type*) {
        // TODO: This is not done yet
        e.push(keg::WriterParam::BEGIN_SEQUENCE);

        e.push(keg::WriterParam::END_SEQUENCE);
        return true;
    }
}
