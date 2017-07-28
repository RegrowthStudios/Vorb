#include "stdafx.h"
#include "Delegate.hpp"

namespace DelegateDestructor {
    extern std::vector<Destructor> g_destructors{};
}

size_t DelegateDestructor::genDestructorID(DelegateDestructor::Destructor f) {
    size_t i = DelegateDestructor::g_destructors.size();
    DelegateDestructor::g_destructors.emplace_back(f);
    return i;
}

void DelegateDestructor::destroy(void* pObject, size_t id) {
    DelegateDestructor::Destructor dtor = DelegateDestructor::g_destructors[id];
    dtor(pObject);
}
