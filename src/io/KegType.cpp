#include "stdafx.h"
#include "io/KegType.h"

#include "io/Keg.h"

keg::Type::Type() :
    m_sizeInBytes(0),
    m_values() {
    // Empty
}
keg::Type::Type(const nString& name, Environment* env) {
    if (env) env->addType(name, this);
    keg::getGlobalEnvironment()->addType(name, this);
}

void keg::Type::addValue(const nString& name, const Value& type) {
    m_values[name] = type;
}
void keg::Type::addSuper(const Type* type, size_t offset /*= 0*/) {
    for (auto kvPair : type->m_values) {
        kvPair.second.offset += offset;
        m_values[kvPair.first] = kvPair.second;
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
