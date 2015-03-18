#include "stdafx.h"
#include "io/KegType.h"

#include "io/Keg.h"

keg::Type::Type() :
_sizeInBytes(0),
_values() {
    // Empty
}
keg::Type::Type(const nString& name, Environment* env) {
    if (env) env->addType(name, this);
    keg::getGlobalEnvironment()->addType(name, this);
}

void keg::Type::addValue(const nString& name, const Value& type) {
    _values[name] = type;
}
void keg::Type::addSuper(const Type* type, size_t offset /*= 0*/) {
    for (auto kvPair : type->_values) {
        kvPair.second.offset += offset;
        _values[kvPair.first] = kvPair.second;
    }
}

void keg::Type::construct(void* data) {
    m_ctor(data);
}
void* keg::Type::alloc() {
    return m_alloc();
}
void* keg::Type::allocArray(size_t s) {
    return m_allocArray(s);
}
keg::DeallocatorFunction keg::Type::getDeallocator() const {
    return m_deallocArray;
}
