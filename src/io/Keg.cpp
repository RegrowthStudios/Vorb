#include "stdafx.h"
#include "io/Keg.h"

#include "io/YAML.h"

#define KEG_DOC_TYPE_ID "__TYPE__"
#define KEG_DOC_DATA_ID "__DATA__"

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


    Environment::Environment() :
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
    _internalTypes[(i32)BasicType::TYPE##_V2].setStructType<C_TYPE##v2>(); \
    kv = Value::basic(sizeof(C_TYPE)* 1, BasicType::TYPE); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("value1", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("y", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("g", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("t", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V2].addValue("1", kv); \
    addType(#TYPE"_V2", &_internalTypes[(i32)BasicType::TYPE##_V2]); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addSuper(&_internalTypes[(i32)BasicType::TYPE##_V2]); \
    _internalTypes[(i32)BasicType::TYPE##_V3].setStructType<C_TYPE##v3>(); \
    kv = Value::basic(sizeof(C_TYPE)* 2, BasicType::TYPE); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("value2", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("z", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("b", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("p", kv); \
    _internalTypes[(i32)BasicType::TYPE##_V3].addValue("2", kv); \
    addType(#TYPE"_V3", &_internalTypes[(i32)BasicType::TYPE##_V3]); \
    _internalTypes[(i32)BasicType::TYPE##_V4].addSuper(&_internalTypes[(i32)BasicType::TYPE##_V3]); \
    _internalTypes[(i32)BasicType::TYPE##_V4].setStructType<C_TYPE##v4>(); \
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
    ui32 Environment::addType(const nString& name, Type* type) {
        _uuid++;
        _typesByName[name] = type;
        _typesByID[_uuid] = type;
        return _uuid;
    }
    ui32 Environment::addEnum(const nString& name, Enum* type) {
        _uuid++;
        _enumsByName[name] = type;
        _enumsByID[_uuid] = type;
        return _uuid;
    }

    Error parse(ui8* dest, keg::Node& data, keg::YAMLReader& doc, Environment* env, Type* type);
    Error parse(void* dest, const cString data, Type* type, Environment* env /*= nullptr*/) {
        // Test Arguments
        if (env == nullptr) env = getGlobalEnvironment();
        if (dest == nullptr || type == nullptr || data == nullptr) {
            return Error::BAD_ARGUMENT;
        }

        // Parse YAML
        keg::YAMLReader doc;
        doc.init(data);
        keg::Node baseNode = doc.getFirst();
        if (keg::getType(baseNode) == keg::NodeType::NONE) return Error::EARLY_EOF;

        // Parse
        Error err = parse((ui8*)dest, baseNode, doc, env, type);
        doc.dispose();
        return err;
    }
    Error parse(void* dest, const cString data, const nString& typeName, Environment* env /*= nullptr*/) {
        // Test Arguments
        if (env == nullptr) env = getGlobalEnvironment();
        if (dest == nullptr || typeName.empty() || data == nullptr) {
            return Error::BAD_ARGUMENT;
        }

        // Attempt To Find The Type
        Type* type = env->getType(typeName);
        if (type == nullptr) return Error::TYPE_NOT_FOUND;

        // Parse YAML
        keg::YAMLReader doc;
        doc.init(data);
        keg::Node baseNode = doc.getFirst();
        if (keg::getType(baseNode) == keg::NodeType::NONE) return Error::EARLY_EOF;

        // Parse
        Error err = parse((ui8*)dest, baseNode, doc, env, type);
        doc.dispose();
        return err;
    }
    Error parse(void* dest, const cString data, const ui32& typeID, Environment* env /*= nullptr*/) {
        // Test Arguments
        if (env == nullptr) env = getGlobalEnvironment();
        if (dest == nullptr || typeID == KEG_BAD_TYPE_ID || data == nullptr) {
            return Error::BAD_ARGUMENT;
        }

        // Attempt To Find The Type
        Type* type = env->getType(typeID);
        if (type == nullptr) return Error::TYPE_NOT_FOUND;

        // Parse YAML
        keg::YAMLReader doc;
        doc.init(data);
        keg::Node baseNode = doc.getFirst();
        if (keg::getType(baseNode) == keg::NodeType::NONE) return Error::EARLY_EOF;

        // Parse
        Error err = parse((ui8*)dest, baseNode, doc, env, type);
        doc.dispose();
        return err;
    }

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
            return nullptr;
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

    void evalData(ui8* dest, const Value* decl, keg::Node& node, keg::YAMLReader& doc, Environment* env);

    inline Error evalValueCustom(ui8* dest, keg::Node& value, const Value* decl, keg::YAMLReader& doc, Environment* env) {
        // Test Arguments
        if (decl->typeName.empty()) return Error::TYPE_NOT_FOUND;

        // Attempt To Find The Type
        Type* type = env->getType(decl->typeName);
        if (type == nullptr) return Error::TYPE_NOT_FOUND;

        return parse(dest, value, doc, env, type);
    }
    inline Error evalValueEnum(ui8* dest, keg::Node& value, const Value* decl, keg::YAMLReader& doc, Environment* env) {
        // Test Arguments
        if (decl->typeName.empty()) return Error::TYPE_NOT_FOUND;

        // Attempt To Find The Type
        Enum* type = env->getEnum(decl->typeName);
        if (type == nullptr) return Error::TYPE_NOT_FOUND;
        type->setValue(dest, keg::convert<nString>(value));
        return Error::NONE;
    }
    inline Error evalValuePtr(void** dest, keg::Node& value, const Value* decl, keg::YAMLReader& doc, Environment* env) {
        // The Type We Will Be Allocating
        nString typeName = decl->typeName;
        if (typeName.empty()) {
            auto kvp = basicTypes.find(decl->interiorValue->type);
            if (kvp != basicTypes.end()) typeName = kvp->second;
            else typeName = decl->interiorValue->typeName;
        }

        if (keg::hasInterior(value, KEG_DOC_TYPE_ID)) {
            keg::Node typeNode = doc.getInterior(value, KEG_DOC_TYPE_ID);
            typeName = keg::convert<nString>(typeNode);
            doc.free(typeNode);
        }

        // Test Arguments
        if (typeName.empty()) return Error::TYPE_NOT_FOUND;

        // Attempt To Find The Type
        Type* type = env->getType(typeName);
        if (type == nullptr) return Error::TYPE_NOT_FOUND;

        *dest = type->alloc();
        return parse((ui8*)*dest, value, doc, env, type);
    }
    inline Error evalValueArray(ArrayBase* dest, keg::Node& value, const Value* decl, keg::YAMLReader& doc, Environment* env) {
        nString typeName = decl->typeName;
        if (typeName.empty()) {
            auto kvp = basicTypes.find(decl->interiorValue->type);
            if (kvp != basicTypes.end()) typeName = kvp->second;
            else typeName = decl->interiorValue->typeName;
        }

        keg::Node nArray = nullptr;
        if (keg::getType(value) == keg::NodeType::SEQUENCE) {
            nArray = value;
        } else {
            if (keg::hasInterior(value, KEG_DOC_DATA_ID)) {
                nArray = doc.getInterior(value, KEG_DOC_DATA_ID);
                if (keg::getType(nArray) != keg::NodeType::SEQUENCE) return Error::BAD_VALUE;
                if (keg::hasInterior(value, KEG_DOC_TYPE_ID)) {
                    keg::Node typeNode = doc.getInterior(value, KEG_DOC_TYPE_ID);
                    typeName = keg::convert<nString>(typeNode);
                    doc.free(typeNode);
                }
            }
        }

        // Test Arguments
        if (typeName.empty()) return Error::TYPE_NOT_FOUND;

        // Attempt To Find The Type
        Type* type = env->getType(typeName);
        if (type == nullptr) {
            return Error::TYPE_NOT_FOUND;
        }
        new (dest) ArrayBase(type->getSizeInBytes());
        size_t seqLen = keg::getSequenceSize(nArray);
        if (seqLen > 0) {
            dest->ownData(type->allocArray(seqLen), seqLen, type->getDeallocator());
            ui8* newDest = &dest->at<ui8>(0);

            auto f = makeFunctor<Sender, size_t, keg::Node>([&] (Sender, size_t, keg::Node node) {
                evalData(newDest, decl->interiorValue.get(), node, doc, env);
                doc.free(node);
                newDest += type->getSizeInBytes();
            });
            doc.forAllInSequence(nArray, f);
            delete f;
        }
        return Error::TYPE_NOT_FOUND;
    }

    void evalData(ui8* dest, const Value* decl, keg::Node &node, keg::YAMLReader& doc, Environment* env) {
#define KEG_EVAL_CASE_NUM(ENUM, TYPE) \
        case BasicType::ENUM: *((TYPE*)dest) = keg::convert<TYPE>(node); break; \
        case BasicType::ENUM##_V2: *((TYPE##v2*)dest) = keg::convert<TYPE##v2>(node); break; \
        case BasicType::ENUM##_V3: *((TYPE##v3*)dest) = keg::convert<TYPE##v3>(node); break; \
        case BasicType::ENUM##_V4: *((TYPE##v4*)dest) = keg::convert<TYPE##v4>(node); break
        switch (decl->type) {
            KEG_EVAL_CASE_NUM(I8, i8);
            KEG_EVAL_CASE_NUM(I16, i16);
            KEG_EVAL_CASE_NUM(I32, i32);
            KEG_EVAL_CASE_NUM(I64, i64);
            //KEG_EVAL_CASE_NUM(UI8, ui8);
        case BasicType::UI8: *((ui8*)dest) = (ui8)keg::convert<ui16>(node); break;
        case BasicType::UI8_V2: *((ui8v2*)dest) = keg::convert<ui8v2>(node); break;
        case BasicType::UI8_V3: *((ui8v3*)dest) = keg::convert<ui8v3>(node); break;
        case BasicType::UI8_V4: *((ui8v4*)dest) = keg::convert<ui8v4>(node); break;
            KEG_EVAL_CASE_NUM(UI16, ui16);
            KEG_EVAL_CASE_NUM(UI32, ui32);
            KEG_EVAL_CASE_NUM(UI64, ui64);
            KEG_EVAL_CASE_NUM(F32, f32);
            KEG_EVAL_CASE_NUM(F64, f64);
        case BasicType::BOOL:
            *((bool*)dest) = keg::convert<bool>(node);
            break;
        case BasicType::ARRAY:
            evalValueArray((ArrayBase*)dest, node, decl, doc, env);
            break;
        case BasicType::C_STRING:
        {
                                    nString data = keg::convert<nString>(node);
                                    cString dataCopy =  new char[data.length() + 1];
                                    memcpy(dataCopy, data.c_str(), data.length());
                                    dataCopy[data.length()] = 0;
                                    *((cString*)dest) = dataCopy;
        }
            break;
        case BasicType::STRING:
            *((nString*)dest) = keg::convert<nString>(node); 
            break;
        case BasicType::PTR:
            evalValuePtr((void**)dest, node, decl, doc, env);
            break;
        case BasicType::ENUM:
            evalValueEnum((ui8*)dest, node, decl, doc, env);
            break;
        case BasicType::CUSTOM:
            evalValueCustom((ui8*)dest, node, decl, doc, env);
            break;
        default:
            break;
        }
    }

    Error parse(ui8* dest, keg::Node& data, keg::YAMLReader& doc, Environment* env, Type* type) {
        if (keg::getType(data) != keg::NodeType::MAP) return Error::BAD_VALUE;

        // Attempt To Redefine Type
        if (keg::hasInterior(data, KEG_DOC_TYPE_ID)) {
            keg::Node nodeType = doc.getInterior(data, KEG_DOC_TYPE_ID);
            if (keg::getType(nodeType) == keg::NodeType::VALUE) {
                Type* nType = env->getType(keg::convert<nString>(nodeType));
                if (!nType) type = nullptr;
            }
            doc.free(nodeType);
        }

        // We Need A Type
        if (!type) return Error::TYPE_NOT_FOUND;

        // Iterate Values
        auto f = makeFunctor<Sender, const nString&, keg::Node>([&] (Sender, const nString& valName, keg::Node node) {
            const Value* v = type->getValue(valName);
            if (v) evalData(dest + v->offset, v, node, doc, env);
        });
        doc.forAllInMap(data, f);
        delete f;
        return Error::NONE;
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
            case BasicType::TYPE##_V2: e << C_TYPE##v2(*data); break; \
            case BasicType::TYPE##_V3: e << C_TYPE##v3(*data); break; \
            case BasicType::TYPE##_V4: e << C_TYPE##v4(*data); break
                // For when we want to interpret TYPE as C_TYPE
#define EMIT_NUM(TYPE, C_TYPE) \
            case BasicType::TYPE: e << *(C_TYPE*)data; break; \
            case BasicType::TYPE##_V2: e << *(C_TYPE##v2*)data; break; \
            case BasicType::TYPE##_V3: e << *(C_TYPE##v3*)data; break; \
            case BasicType::TYPE##_V4: e << *(C_TYPE##v4*)data; break
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
