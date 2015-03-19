#include "stdafx.h"
#include "io/Keg.h"

#include "io/YAML.h"
#include "io/KegFuncs.h"

namespace keg {
    Type* getCorrectType(keg::Node& node, ReadContext& context, Type* previous) {
        // Get interior node for the type information
        if (!keg::hasInterior(node, KEG_DOC_TYPE_ID)) return previous;
        keg::Node type = context.reader.getInterior(node, KEG_DOC_TYPE_ID);
        if (keg::getType(type) != NodeType::VALUE) {
            context.reader.free(type);
            return previous;
        }

        // Get name of the conversion type
        nString typeName = keg::convert<nString>(type);
        context.reader.free(type);
        Type* otherType = context.env->getType(typeName);

        // Return correct type
        return otherType ? otherType : previous;
    }

    class TypeValueIterator {
    public:
        TypeValueIterator(ui8* dest, Type* t, ReadContext& context) :
            m_dest(dest),
            m_type(t),
            m_context(&context) {
            // Empty
        }

        void evaluate(Sender, const nString& valName, keg::Node node) {
            const Value* v = m_type->getValue(valName);
            if (v) {
                ui8* valueDest = m_dest + v->offset;
                evalData(valueDest, v, node, *m_context);
            }
        }
    private:
        ReadContext* m_context;
        ui8* m_dest;
        Type* m_type;
    };

    Error parse(void* dest, const cString data, Type* type, Environment* env /*= nullptr*/) {
        // Test Arguments
        if (env == nullptr) env = getGlobalEnvironment();
        if (dest == nullptr || type == nullptr || data == nullptr) {
            return Error::BAD_ARGUMENT;
        }

        // Parse YAML
        ReadContext context;
        context.env = env;
        context.reader.init(data);
        keg::Node baseNode = context.reader.getFirst();
        if (keg::getType(baseNode) == keg::NodeType::NONE) return Error::EARLY_EOF;

        // Parse
        type = getCorrectType(baseNode, context, type);
        Error err = parse((ui8*)dest, baseNode, context, type);
        context.reader.dispose();
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
        ReadContext context;
        context.env = env;
        context.reader.init(data);
        keg::Node baseNode = context.reader.getFirst();
        if (keg::getType(baseNode) == keg::NodeType::NONE) return Error::EARLY_EOF;

        // Parse
        type = getCorrectType(baseNode, context, type);
        Error err = parse((ui8*)dest, baseNode, context, type);
        context.reader.dispose();
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
        ReadContext context;
        context.env = env;
        context.reader.init(data);
        keg::Node baseNode = context.reader.getFirst();
        if (keg::getType(baseNode) == keg::NodeType::NONE) return Error::EARLY_EOF;

        // Parse
        type = getCorrectType(baseNode, context, type);
        Error err = parse((ui8*)dest, baseNode, context, type);
        context.reader.dispose();
        return err;
    }

    inline Error evalValueCustom(ui8* dest, keg::Node& value, const Value* decl, ReadContext& context) {
        // Test Arguments
        if (decl->typeName.empty()) return Error::TYPE_NOT_FOUND;

        // Attempt To Find The Type
        Type* type = context.env->getType(decl->typeName);
        if (type == nullptr) return Error::TYPE_NOT_FOUND;

        return parse(dest, value, context, type);
    }
    inline Error evalValueEnum(ui8* dest, keg::Node& value, const Value* decl, ReadContext& context) {
        // Test Arguments
        if (decl->typeName.empty()) return Error::TYPE_NOT_FOUND;

        // Attempt To Find The Type
        Enum* type = context.env->getEnum(decl->typeName);
        if (type == nullptr) return Error::TYPE_NOT_FOUND;
        type->setValue(dest, keg::convert<nString>(value));
        return Error::NONE;
    }
    inline Error evalValuePtr(void** dest, keg::Node& value, const Value* decl, ReadContext& context) {
        // The Type We Will Be Allocating
        nString typeName = decl->typeName;
        if (typeName.empty()) {
            auto kvp = basicTypes.find(decl->interiorValue->type);
            if (kvp != basicTypes.end()) typeName = kvp->second;
            else typeName = decl->interiorValue->typeName;
        }
        Type* type = context.env->getType(typeName);
        type = getCorrectType(value, context, type);

        // Attempt To Find The Type
        if (type == nullptr) return Error::TYPE_NOT_FOUND;

        *dest = type->alloc();
        return parse((ui8*)*dest, value, context, type);
    }
    inline Error evalValueArray(ArrayBase* dest, keg::Node& value, const Value* decl, ReadContext& context) {
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
                nArray = context.reader.getInterior(value, KEG_DOC_DATA_ID);
                if (keg::getType(nArray) != keg::NodeType::SEQUENCE) return Error::BAD_VALUE;
                if (keg::hasInterior(value, KEG_DOC_TYPE_ID)) {
                    keg::Node typeNode = context.reader.getInterior(value, KEG_DOC_TYPE_ID);
                    typeName = keg::convert<nString>(typeNode);
                    context.reader.free(typeNode);
                }
            }
        }

        // Test Arguments
        if (typeName.empty()) return Error::TYPE_NOT_FOUND;

        // Attempt To Find The Type
        Type* type = context.env->getType(typeName);
        if (type == nullptr) {
            return Error::TYPE_NOT_FOUND;
        }
        new (dest)ArrayBase(type->getSizeInBytes());
        size_t seqLen = keg::getSequenceSize(nArray);
        if (seqLen > 0) {
            dest->ownData(type->allocArray(seqLen), seqLen, type->getDeallocator());
            ui8* newDest = &dest->at<ui8>(0);

            auto f = makeFunctor<Sender, size_t, keg::Node>([&] (Sender, size_t, keg::Node node) {
                evalData(newDest, decl->interiorValue.get(), node, context);
                context.reader.free(node);
                newDest += type->getSizeInBytes();
            });
            context.reader.forAllInSequence(nArray, f);
            delete f;
        }
        return Error::TYPE_NOT_FOUND;
    }

    Error parse(ui8* dest, keg::Node& data, ReadContext& context, Type* type) {
        // Check arguments
        if (!type) return Error::TYPE_NOT_FOUND;
        if (keg::getType(data) != keg::NodeType::MAP) return Error::BAD_VALUE;

        // Iterate Values
        TypeValueIterator typeEval(dest, type, context);
        auto eval = makeDelegate(typeEval, &TypeValueIterator::evaluate);
        context.reader.forAllInMap(data, &eval);
        return Error::NONE;
    }
    void evalData(ui8* dest, const Value* decl, keg::Node &node, ReadContext& context) {
        if (decl->evaluator) {
            decl->evaluator(dest, node);
        } else {
            switch (decl->type) {
            case BasicType::PTR:
                evalValuePtr((void**)dest, node, decl, context);
                break;
            case BasicType::ARRAY:
                evalValueArray((ArrayBase*)dest, node, decl, context);
                break;
            case BasicType::ENUM:
                evalValueEnum(dest, node, decl, context);
                break;
            case BasicType::CUSTOM:
                evalValueCustom(dest, node, decl, context);
                break;
            default:
                break;
            }
        }
    }
}
